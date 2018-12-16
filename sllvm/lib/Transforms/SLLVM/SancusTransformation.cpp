#include "SancusTransformation.h"
#include "Analysis.h"
#include "../lib/Target/MSP430/MSP430Sancus.h"
#include "llvm/IR/CallSite.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/SLLVM.h"

#define DEBUG_TYPE "sllvm"

using namespace sllvm;

static std::pair<Function *, Value *>
getOrCreateEEntryStub(Module &M, const Function *F) {
  assert(F != nullptr);
  Type * Int16Ty = Type::getInt16Ty(M.getContext());
  FunctionType *FTy = cast<FunctionType>(F->getValueType());

  SmallVector<Type *, 6> P {Int16Ty};
  P.insert(std::end(P), FTy->params().begin(), FTy->params().end());
  FunctionType *Ty = FunctionType::get(FTy->getReturnType(), P, false);

  //TODO: Have function names produced by function
  Function *S = dyn_cast<Function>(M.getOrInsertFunction(
      Twine("sllvm_", F->getName()).str(), Ty));
  assert(S != nullptr);
  S->setCallingConv(CallingConv::SANCUS_ENTRY);

  //TODO: Have symbol names produced by function
  GlobalVariable *GV = dyn_cast<GlobalVariable>(M.getOrInsertGlobal(
      Twine("sllvm_id_", F->getName()).str(), Int16Ty));
  GV->setConstant(true);

  return std::make_pair(S, GV);
}

static GlobalVariable *newSecretVariable(Module &M, const char *N, Type *T) {
  auto result = new GlobalVariable(M,
      T,
      false,
      //TODO: GlobalVariable::LinkageTypes::PrivateLinkage,
      GlobalVariable::LinkageTypes::InternalLinkage,
      Constant::getNullValue(T),
      N);
  // TODO Have section names generated
  result->setSection(Twine(".sllvm.data.", getPMName(&M)).str());
  return result;
}

static GlobalVariable *newMACVariable(Module &M, const Function *F) {
  // TODO: Rename MAC to hash?
  // TODO: Store MAC/HASH in protected text section?
  LLVMContext &Ctx = M.getContext();
  IRBuilder<> IRB(Ctx);
  Type *Int8Ty = IRB.getInt8Ty();
  // TODO: MAC size should be command-line argument
  Type *ArTy = ArrayType::get(Int8Ty, 8);

  // TODO Have the MAC variable name generated
  auto GV = M.getNamedGlobal(
      ("sllvm_mac_" + getPMName(&M) + "_" + F->getName()).str());

  auto result = new GlobalVariable(M,
      ArTy,
      true,
      GlobalVariable::LinkageTypes::InternalLinkage,
      Constant::getNullValue(ArTy),
      // TODO Have the MAC variable name generated
      // TODO ADD module name to uniqufy if linkage has to be external !!!!
      "sllvm_mac_" + getPMName(&M) + "_" + F->getName());
  // TODO Have section names generated
  result->setSection(
      (".sllvm.mac." + getPMName(&M) + "." + F->getName()).str());

  if (GV != nullptr) {
    // TODO: Handle existing definitions more gracefully
    //        (also when they come from extern declaration in C)
    //assert(GV->getType() == ArTy && "Unexpected type");
    GV->replaceAllUsesWith(result);
    GV->eraseFromParent();
    result->setName("sllvm_mac_" + getPMName(&M) + "_" + F->getName());
  }

  return result;
}

void SancusTransformation::handleGlobals(Module &M) {
  LLVMContext &Ctx = M.getContext();
  IRBuilder<> IRB(Ctx);

  Type *Int16Ty = IRB.getInt16Ty();
  // TODO: Stack size should be command-line argument
  Type *ArTy = ArrayType::get(Int16Ty, 128);
  auto S = newSecretVariable(M, sancus::local_stack, ArTy);

  Constant *IdxList[] = {
    ConstantInt::get(Int16Ty, 0),
    // TODO: Stack size should be command-line argument
    ConstantInt::get(Int16Ty, 127)
  };

  auto C = ConstantExpr::getGetElementPtr(nullptr, S, IdxList);

  auto GV = newSecretVariable(M, sancus::local_r1, C->getType());
  GV->setInitializer(C);

  newSecretVariable(M, sancus::local_r4, Int16Ty);
  newSecretVariable(M, sancus::local_r5, Int16Ty);
  newSecretVariable(M, sancus::local_r8, Int16Ty);
  newSecretVariable(M, sancus::local_r9, Int16Ty);
  newSecretVariable(M, sancus::local_r10, Int16Ty);
  newSecretVariable(M, sancus::local_r11, Int16Ty);

  for (auto GVI = M.global_begin(), E = M.global_end(); GVI != E; GVI++) {
    if (getAnalysis<SLLVMAnalysis>().getResults().isEData(&*GVI)) {
      //TODO: GVI->setLinkage(GlobalVariable::LinkageTypes::PrivateLinkage);
      GVI->setLinkage(GlobalVariable::LinkageTypes::InternalLinkage);
    }
  }
}

void SancusTransformation::createDispatchBody(Module &M, Function *D) {
  LLVMContext &C = M.getContext();
  IRBuilder<> IRB(C);
  IntegerType * Int16Ty = IRB.getInt16Ty();

  // TODO: Have function generate label names
  BasicBlock *EntryBB = BasicBlock::Create(C, "entry", D);
  BasicBlock *DefBB = BasicBlock::Create(C, "sw.default", D);
  BasicBlock *EpiBB = BasicBlock::Create(C, "sw.epilog", D);
  BasicBlock *ResumeBB = BasicBlock::Create(C, "sw.eresume", D);

  IRB.SetInsertPoint(EntryBB);
  IRB.CreateCall(Intrinsic::getDeclaration(&M, Intrinsic::sllvm_eenter));
  SwitchInst *SI = IRB.CreateSwitch(D->arg_begin(), DefBB, 5);

  IRB.SetInsertPoint(DefBB);
  IRB.CreateBr(EpiBB); // TODO: Exit instead of EpiBB

  IRB.SetInsertPoint(EpiBB);
  IRB.CreateCall(Intrinsic::getDeclaration(&M, Intrinsic::sllvm_eexit));
  IRB.CreateRetVoid();

  IRB.SetInsertPoint(ResumeBB);
  IRB.CreateCall(Intrinsic::getDeclaration(&M, Intrinsic::sllvm_eresume));
  IRB.CreateRetVoid(); // TODO: Remove

  SI->addCase(ConstantInt::get(Int16Ty, sancus::R6_PRet), ResumeBB);
  SI->addCase(ConstantInt::get(Int16Ty, sancus::R6_URet), ResumeBB);

  int id = 1;
  for (Function& F : M) {
    if (getAnalysis<SLLVMAnalysis>().getResults().isEEntryDef(&F)) {
      // TODO: Have function generate label names
      BasicBlock * BB = BasicBlock::Create(C, Twine("sw.", F.getName()), D);
      SI->addCase(ConstantInt::get(Int16Ty, id), BB);
      IRB.SetInsertPoint(BB);

      // TODO: There should be a more idiomatic way to do this
      //   -> use Argument::getArgNo()
      std::vector<Value *> args;
      auto DAI = D->arg_begin();
      auto FAI = F.arg_begin();
      for (unsigned i = 0, e = F.arg_size(); i != e; ++i) {
        DAI++;
        assert(DAI != D->arg_end());
        assert(FAI != F.arg_end());
        if (FAI->getType()->isPointerTy()) {
          args.push_back(IRB.CreateIntToPtr(DAI, FAI->getType()));
        }
        else {
          args.push_back(DAI);
        }
        FAI++;
      }

      IRB.CreateCall(&F, args);
      //TODO Optimise as follows
      //     CI->setCallingConv(CallingConv::SANCUS_DISPATCH);
      //        (CI is the result of CreateCall)
      // For now, skip this optimization as this confuses the register 
      // allocator when it is dealing with the dispatcher having more
      // than 3 branches. (When the dispatcher has more than 3 branches
      // LLVM optimizes the generated code in such a way that it uses
      // R12 and R13, overwriting the input parameters)
      IRB.CreateBr(EpiBB);

      // TODO: Have symbols generated by function
      GlobalAlias::create(GlobalVariable::LinkageTypes::ExternalLinkage,
          Twine("sllvm_", F.getName()),
          D);

      // TODO: Put this in a section that is not part of the PM (ASK JT)
      new GlobalVariable(M,
          Int16Ty,
          true,
          GlobalVariable::LinkageTypes::ExternalLinkage,
          ConstantInt::get(Int16Ty, id),
          // TODO: Have symbol names produced by a function 
          Twine("sllvm_id_", F.getName()));

      //TODO: F.setLinkage(GlobalVariable::LinkageTypes::PrivateLinkage);
      F.setLinkage(GlobalVariable::LinkageTypes::InternalLinkage);

      id++;
    }
  }
}

// Generate code to support the officially supported (aka legacy) API,
// required for compiling the example code of the officially supported 
// (aka legacy) Sancus development environment
void SancusTransformation::supportLegacyAPI(Module &M) {
  LLVMContext &C = M.getContext();

  // TODO: Have typenames generated
  // SancusModule
  auto SancusModuleTy = M.getTypeByName("struct.SancusModule");
  if (SancusModuleTy == nullptr) {
    SancusModuleTy = StructType::create(C, 
    { 
      Type::getInt16Ty(C), 
      Type::getInt16Ty(C), 
      Type::getInt8PtrTy(C),
      Type::getInt8PtrTy(C),
      Type::getInt8PtrTy(C),
      Type::getInt8PtrTy(C),
      Type::getInt8PtrTy(C) 
    }, 
    "struct.SancusModule");
  }
  else {
    // Type is defined by including "sm_support.h" 
    //    (declares part of the legacy API)
    //
    // TODO: Assert type is as expected 
    //            {int16, int16, int8*, int8*, int8*, int8*, int8*}
  }

  auto V = ConstantDataArray::getString(M.getContext(), getPMName(&M));
  auto GV = new GlobalVariable(
      M, V->getType(), true, GlobalValue::PrivateLinkage, V);
  // TODO add 'unnamed_addr' attribute
  GV->setAlignment(1);
  // TODO: GV Should not be part of the secret section

  Constant *IdxList[] = {
    Constant::getNullValue(Type::getInt8Ty(C)),
    Constant::getNullValue(Type::getInt8Ty(C))
  };

  // TODO: Have symbol names generated
  // TODO: Paramterize (vendor_id)
  Constant *Vals[] = {
    ConstantInt::get(Type::getInt16Ty(C), 0),
    ConstantInt::get(Type::getInt16Ty(C), 0x1234), // Vendor id
    ConstantExpr::getInBoundsGetElementPtr(V->getType(), GV, IdxList),

    M.getOrInsertGlobal(
        Twine("sllvm_pm_" + getPMName(&M) + "_text_start").str(),
        Type::getInt8Ty(C)),
    M.getOrInsertGlobal(
        Twine("sllvm_pm_" + getPMName(&M) + "_text_end").str(),
        Type::getInt8Ty(C)),
    M.getOrInsertGlobal(
        Twine("sllvm_pm_" + getPMName(&M) + "_data_start").str(),
        Type::getInt8Ty(C)),
    M.getOrInsertGlobal(
        Twine("sllvm_pm_" + getPMName(&M)+ "_data_end").str(),
        Type::getInt8Ty(C))
  };

  auto Init = ConstantStruct::get(SancusModuleTy, Vals);
  GV = dyn_cast<GlobalVariable>(
      M.getOrInsertGlobal(getPMName(&M), SancusModuleTy));
  GV->setConstant(false);
  GV->setLinkage(GlobalValue::ExternalLinkage);
  GV->setInitializer(Init);
  GV->setDSOLocal(true);
  GV->setAlignment(2);
}

void SancusTransformation::createDispatch(Module &M) {
  Type * VoidTy = Type::getVoidTy(M.getContext());
  Type * Int16Ty = Type::getInt16Ty(M.getContext());
  Type *P[] = {Int16Ty, Int16Ty, Int16Ty, Int16Ty, Int16Ty};
  FunctionType *FTy = FunctionType::get(VoidTy, P, false);
  Function *F = Function::Create(FTy,
      //TODO: GlobalValue::PrivateLinkage,
      GlobalValue::InternalLinkage,
      sancus::fname_dispatch,
      &M);

#if 0 // Parameters names are not necessary
  //TODO: Have param names produced by function
  //TODO: There should be a better way to set the argument names...
  auto AI = F->arg_begin();
  AI->setName("id"); AI++; assert(AI != F->arg_end());
  AI->setName("p1"); AI++; assert(AI != F->arg_end());
  AI->setName("p2"); AI++; assert(AI != F->arg_end());
  AI->setName("p3"); AI++; assert(AI != F->arg_end());
  AI->setName("p4"); AI++; assert(AI == F->arg_end());
#endif

  F->setCallingConv(CallingConv::SANCUS_ENTRY);

  createDispatchBody(M, F);
}

void SancusTransformation::handleCalls(Module &M) {
  LLVMContext &C = M.getContext();
  IRBuilder<> IRB(C);

  std::vector<Instruction *> EECalls;

  for (Function& F : M) {
    for (BasicBlock &BB : F) {
      for (Instruction &I : BB) {
        auto CS = CallSite(&I);

        if (getAnalysis<SLLVMAnalysis>().getResults().isEExitCall(&I)) {
          if (getAnalysis<SLLVMAnalysis>().getResults().isEEntryCall(&I)) {
            const Function *C = CS.getCalledFunction();
            newMACVariable(M, C);
          }
          IRB.SetInsertPoint(&I);
          IRB.CreateCall(
              Intrinsic::getDeclaration(&M, Intrinsic::sllvm_excall));
        }

        if (getAnalysis<SLLVMAnalysis>().getResults().isEEntryCall(&I)) {
          const Function *C = CS.getCalledFunction();

          EECalls.push_back(&I);

          auto S = getOrCreateEEntryStub(M, C);

          IRB.SetInsertPoint(&I);
          SmallVector<Value *, 6> A {IRB.CreateLoad(S.second)};
          A.insert(std::end(A), CS.arg_begin(), CS.arg_end());

          CallInst * CI = IRB.CreateCall(S.first, A);
          CI->setCallingConv(S.first->getCallingConv());
          I.replaceAllUsesWith(CI);
        }
      }
    }
  }

  for (auto I : EECalls) {
    I->eraseFromParent();
  }
}

void SancusTransformation::handleEnclave(Module &M) {
  if (getAnalysis<SLLVMAnalysis>().getResults().isPM()) {
    handleGlobals(M);
    createDispatch(M);
    supportLegacyAPI(M);
  }
}

void SancusTransformation::setSections(Module &M) {
  // Globals
  for (auto GVI = M.global_begin(), E = M.global_end(); GVI != E; GVI++) {
    if (getAnalysis<SLLVMAnalysis>().getResults().isEData(&*GVI)) {
      if (GVI->isConstant()) {
        // TODO Have section names generated
        GVI->setSection(Twine(".sllvm.text.", getPMName(&M)).str());
      }
      else {
        // TODO Have section names generated
        GVI->setSection(Twine(".sllvm.data.", getPMName(&M)).str());
      }
    }
  }

  // Functions
  for (Function& F : M) {
    if (F.hasLocalLinkage()) { // TODO: Use SLLVMAnalysis for this (or sllvm.h?)
      // TODO Have section names generated
      F.setSection(Twine(".sllvm.text.", getPMName(&M)).str());
    }
  }

  // Make sure that the dispatch function is put in the beginning of the 
  // protected module's text section (TODO: Is there a better way to do this?)
  // TODO Have section names generated
  Function *F = M.getFunction(sancus::fname_dispatch);
  assert(F != nullptr);
  F->setSection(Twine(".sllvm.text.dispatch.", getPMName(&M)).str());
}

// TODO: Refactor/rename the top level functions called from runOnModule
bool SancusTransformation::runOnModule(Module &M) {
  handleEnclave(M);
  handleCalls(M);

  Type * Int16Ty = Type::getInt16Ty(M.getContext());
  M.getOrInsertGlobal(sancus::global_r1, Int16Ty);
  //if (! getAnalysis<SLLVMAnalysis>().getResults().isPM()) {
    M.getOrInsertGlobal(sancus::global_pc, Int16Ty); // TODO: Remove
  //}

  if (getAnalysis<SLLVMAnalysis>().getResults().isPM()) {
    setSections(M);
  }

  return true; // TODO: Only return true when something changed
}

char SancusTransformation::ID = 0;

static RegisterPass<SancusTransformation> X(
    "sancus-transform", "Sancus transformation pass");

void SancusTransformation::getAnalysisUsage(AnalysisUsage &A) const {
  A.addRequired<SLLVMAnalysis>();
}
