#include <iostream>
#include "SGXTransformation.h"
#include "Analysis.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/IRBuilder.h"

#define DEBUG_TYPE "sllvm"

using namespace sllvm;

// TODO: Should sgx_ocall, sgx_ocalloc, sgx_ocfree be instrinsics ?

//TODO: Have names produced by function (look for strings)
//
//TODO: Use Module::getDataLayout() to fix the following:
//  - Do not hardcode the primitive types as they depend on choices from 
//        - SGX API (sgx_ocall, sgx_ocalloc, sgx_ocfree signatures)
//        - Target platform (e.g. "size_t" is i64 on my machine)
//  - Determine alignment rules

// TODO: Generate extra code to protect program from vulnerabilities when
//       pointers are passed as params to entry points or external functions.
//        (see Intel SGX)

//TODO: This code is vulnerable as it depends on a deterministic processing 
//       order of the
//          - enclave entry functions (Analysis::getEntryPoints())
//          - functions external to enclave (Analysis::getExternalFunctions())
//          - function parameters
//
// The order is relevant because of the 
//          - g_ecall_table struct
//          - g_ocall_table struct
//          - the generated stub parameters where the function argument's 
//            position is mapped onto the member struct's position.
//
// To improve the reliability of this code, we should get rid of this 
// dependency and make the mappings explicit instead of implicitly depending 
// on the iteration order.

void SGXTransformation::generateDynEntryTable(Module &M) {

  LLVMContext &C = M.getContext();
  auto A = getAnalysis<SLLVMAnalysis>().getResults();

  GlobalVariable * GV = nullptr;
  auto L = GlobalVariable::ExternalLinkage;

  if (A.getExternalFunctions().size() == 0) {
    auto DTTy = StructType::create(C, { Type::getInt64Ty(C) }, "struct.DT");
    auto Init = ConstantAggregateZero::get(DTTy);
    GV = new GlobalVariable(M, DTTy, true, L, Init, "g_dyn_entry_table");
  }
  else {
    // (nr of entry poinst x nr external functions) matrix type 
    auto MatrixTy = ArrayType::get(
        ArrayType::get(Type::getInt8Ty(C), A.getExternalFunctions().size()),
        A.getExternalFunctions().size());
    auto DTTy = StructType::create(
        C, { Type::getInt64Ty(C), MatrixTy }, "struct.DT");
    Constant *DTVals[] = {
      ConstantInt::get(Type::getInt64Ty(C), A.getExternalFunctions().size()),
      ConstantAggregateZero::get(MatrixTy),
    };
    auto Init = ConstantStruct::get(DTTy, DTVals);
    GV = new GlobalVariable(M, DTTy, true, L, Init, "g_dyn_entry_table");
  }

  assert(GV != nullptr);

  GV->setDSOLocal(true);
  GV->setAlignment(8);
}

void SGXTransformation::generateECallTable(Module &M) {

  LLVMContext &C = M.getContext();
  auto A = getAnalysis<SLLVMAnalysis>().getResults();

  // 1. Generate ECall table entries (ETE)
  auto ETETy = StructType::create(
      C, { Type::getInt8PtrTy(C), Type::getInt8Ty(C) }, "struct.ETE");

  std::vector<Constant *> ETEs;
  for (const Function *F : A.getEntryPoints()) {
    Function *S = M.getFunction(Twine("sllvm_", F->getName()).str());
    Constant *ETEVals[] = {
      ConstantExpr::getBitCast(S, Type::getInt8PtrTy(C)),
      Constant::getNullValue(Type::getInt8Ty(C))
    };
    ETEs.push_back(ConstantStruct::get(ETETy, ETEVals));
  }
  auto ETEArrTy = ArrayType::get(ETETy, ETEs.size());

  // 2. Generate ECall table (ET)
  auto ETTy = StructType::create(
      C, { Type::getInt64Ty(C), ETEArrTy }, "struct.ET");

  Constant *ETVals[] = {
    ConstantInt::get(Type::getInt64Ty(C), ETEs.size()),
    ConstantArray::get(ETEArrTy, ETEs)
  };

  auto Init = ConstantStruct::get(ETTy, ETVals);
  auto GV = new GlobalVariable(
      M, ETTy, true, GlobalValue::ExternalLinkage, Init, "g_ecall_table");
  GV->setDSOLocal(true);
  GV->setAlignment(8);
}

Type * SGXTransformation::getStubParamType(const Function *F) {
  // TODO: Support more types
  //   For now, only primitive types are supported as aggregate types
  //   different processing in the generated stubs
  assert(F->getParent() != nullptr);
  LLVMContext &C = F->getParent()->getContext();
  Type * Ty = Type::getInt8PtrTy(C);
  std::vector<Type *> Tys;
  if (!F->getReturnType()->isVoidTy()) {
    assert(F->getReturnType()->isSingleValueType());
    Tys.push_back(F->getReturnType());
  }
  for (auto &Arg : F->args()) { // TODO: Is arg ordering deterministic?
                                //         (required by generateStubs())
    assert(Arg.getType()->isSingleValueType());
    Tys.push_back(Arg.getType());
  }
  if (! Tys.empty()) {
    Ty = StructType::create(C, Tys, Twine("struct.", F->getName()).str());
  }
  return Ty;
}

void SGXTransformation::generateStubs(Module &M) {
  LLVMContext &C = M.getContext();
  IRBuilder<> IRB(C);
  auto A = getAnalysis<SLLVMAnalysis>().getResults();

  // Entry points
  for (const Function *F : A.getEntryPoints()) {
    Function *S = Function::Create(
        FunctionType::get(IRB.getInt32Ty(), { IRB.getInt8PtrTy() }, false),
        GlobalValue::InternalLinkage, //TODO: PrivateLinkage,
        Twine("sllvm_", F->getName()),
        &M);
    BasicBlock *BB = BasicBlock::Create(C, "entry", S);
    IRB.SetInsertPoint(BB);
    std::vector<Value *> args;
    auto SPTy = getStubParamType(F);
    if ( (!F->arg_empty()) || (!F->getReturnType()->isVoidTy())) {
      auto SP = IRB.CreateBitCast(S->arg_begin(), SPTy->getPointerTo());
      // TODO: Make sure the argument order is deterministic
      //         (see also getStubParamType() for this)
      unsigned offset = + (F->getReturnType()->isVoidTy() ? 0 : 1);
      for (auto &Arg : F->args()) {
        unsigned idx = Arg.getArgNo() + offset;
        args.push_back(IRB.CreateLoad(
              IRB.CreateConstInBoundsGEP2_32(SPTy, SP, 0, idx)));
      }
    }
    if (F->getReturnType()->isVoidTy()) {
      IRB.CreateCall(const_cast<Function *>(F), args);
    }
    else {
      auto SP = IRB.CreateBitCast(S->arg_begin(), SPTy->getPointerTo());
      IRB.CreateStore(
          IRB.CreateCall(const_cast<Function *>(F), args),
          IRB.CreateConstInBoundsGEP2_32(SPTy, SP, 0, 0));
    }
    IRB.CreateRet(Constant::getNullValue(IRB.getInt32Ty()));
  }

  // External functions
  unsigned Idx = 0;
  for (const Function *F : A.getExternalFunctions()) {

    auto FTy = F->getFunctionType();
    SmallVector<Type *, 6> Tys;
    if (!F->getReturnType()->isVoidTy()) {
      Tys.push_back(FTy->getReturnType()->getPointerTo());
    }
    Tys.insert(std::end(Tys), FTy->params().begin(), FTy->params().end());
    FunctionType *STy = FunctionType::get(IRB.getInt32Ty(), Tys, false);
    auto SName = Twine("sllvm_", F->getName());
    Function *S = Function::Create(
        STy, GlobalValue::InternalLinkage, SName.str(), &M);

    BasicBlock *BB = BasicBlock::Create(C, "entry", S);
    IRB.SetInsertPoint(BB);

    FTy = FunctionType::get(
        IRB.getInt32Ty(), {IRB.getInt32Ty(), IRB.getInt8PtrTy() }, false);
    Function * OCall = dyn_cast<Function>(
        M.getOrInsertFunction("sgx_ocall", FTy));

    if ( F->arg_empty() && (F->getReturnType()->isVoidTy()) ) {
      Value *Vs[] = {
          ConstantInt::get(IRB.getInt32Ty(), Idx),
          Constant::getNullValue(IRB.getInt8PtrTy())
      };
      IRB.CreateRet(IRB.CreateCall(OCall, Vs));
    }
    else {
      auto OCTy = getStubParamType(F);
      assert(M.getDataLayout().isLegalInteger(64)); // TODO
      Value * V;
      V = ConstantInt::get(
          Type::getInt64Ty(C), M.getDataLayout().getTypeAllocSize(OCTy));

      FTy = FunctionType::get(IRB.getInt8PtrTy(), { V->getType() }, false);
      Function * OCAlloc = dyn_cast<Function>(
          M.getOrInsertFunction("sgx_ocalloc", FTy));

      auto P = IRB.CreateCall(OCAlloc, { V });
      // TODO: Check result of sgx_ocalloc
      auto OCP = IRB.CreateBitCast(P, OCTy->getPointerTo());
      // TODO: Is arg ordering deterministic?
      //   Design a safer way of mapping the function arguments to the
      //   struct members.
      
      auto AI = S->arg_begin();
      if (!F->getReturnType()->isVoidTy()) AI++;
      while (AI != S->arg_end()) {
        unsigned idx = AI->getArgNo();
        IRB.CreateStore(
            const_cast<Argument *>(AI), 
            IRB.CreateConstInBoundsGEP2_32(OCTy, OCP, 0, idx));
        AI++;
      }

      Value *Vs[] = { ConstantInt::get(IRB.getInt32Ty(), Idx), P };
      auto R = IRB.CreateCall(OCall, Vs);
      if (!F->getReturnType()->isVoidTy()) {
        IRB.CreateStore(
            IRB.CreateLoad(IRB.CreateConstInBoundsGEP2_32(OCTy, OCP, 0, 0)),
            const_cast<Argument *>(S->arg_begin()));
      }
      FTy = FunctionType::get(IRB.getVoidTy(), {}, false);
      Function * OCFree = dyn_cast<Function>(
          M.getOrInsertFunction("sgx_ocfree", FTy));

      IRB.CreateCall(OCFree);
      IRB.CreateRet(R);
    }
    Idx++;
  }
}

// TODO: Factor out common code with SancusTransformation::handleCalls()
//       and put this in common ancestor
void SGXTransformation::handleOutCalls(Module &M) {
  LLVMContext &C = M.getContext();
  auto A = getAnalysis<SLLVMAnalysis>().getResults();
  IRBuilder<> IRB(C);

  std::vector<Instruction *> OCalls;

  for (Function& F : M) {
    for (BasicBlock &BB : F) {
      for (Instruction &I : BB) {
        if (A.isEExitCall(&I)) {

          auto CS = CallSite(&I);
          const Function *CF = CS.getCalledFunction();

          OCalls.push_back(&I);

          Function * S = M.getFunction(Twine("sllvm_", CF->getName()).str());
          assert(S != nullptr);

          IRB.SetInsertPoint(&I);
          Value * R = nullptr;
          std::vector<Value *> A;
          if (!CF->getReturnType()->isVoidTy()) {
            Value * PR;
            R = IRB.CreateAlloca(CF->getReturnType());
            PR = IRB.CreateAlloca(CF->getReturnType()->getPointerTo());
            IRB.CreateStore(R, PR);
            A.push_back(IRB.CreateLoad(PR));
            R = IRB.CreateLoad(R);
          }
          A.insert(std::end(A), CS.arg_begin(), CS.arg_end());

          //auto CI = IRB.CreateCall(S, A);
          IRB.CreateCall(S, A);
          // TODO: Generate code for checking the result of the call

          if (!CF->getReturnType()->isVoidTy()) {
            assert(R != nullptr);
            I.replaceAllUsesWith(R);
          }
        }
      }
    }
  }

  for (auto I : OCalls) {
    I->eraseFromParent();
  }
}

bool SGXTransformation::runOnModule(Module &M) {
  auto A = getAnalysis<SLLVMAnalysis>().getResults();
  if (A.isPM()) {
    if (A.getEntryPoints().size() > 0) {
      generateStubs(M); // Proxies and bridges
      generateECallTable(M);
      generateDynEntryTable(M);
      handleOutCalls(M);
    }
    else {
      // TODO: There should be at least one ECALL (edger8r also enforces this)
    }
  }

  return true; // TODO: Only return true when something changed
}

char SGXTransformation::ID = 0;

static RegisterPass<SGXTransformation> X(
    "sgx-transform", "SGX transformation pass");

void SGXTransformation::getAnalysisUsage(AnalysisUsage &A) const {
  A.addRequired<SLLVMAnalysis>();
}
