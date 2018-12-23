#include "Analysis.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/CallSite.h"
#include "llvm/SLLVM.h"

#define DEBUG_TYPE "sllvm"

using namespace sllvm;

char SLLVMAnalysis::ID = 0;

static RegisterPass<SLLVMAnalysis> X("sllvm-analyze", "SLLVM analysis pass");

bool SLLVMAnalysis::runOnModule(Module &M) {
  R.reset(new SLLVMAnalysisResults(M));
  return false;
}

SLLVMAnalysisResults::SLLVMAnalysisResults(const Module &M) : _isPM(false) {
  for (const Function& F : M) {
    if ( (! F.isDeclaration()) && sllvm::isEEntry(&F)) {
      assert(!F.hasLocalLinkage()); // TODO: Enforce this in CFE
      _isPM = true;
      EEDefs.insert(&F);
    }
  }

  for (const Function& F : M) {
    for (const BasicBlock &BB : F) {
      for (const Instruction &I : BB) {
        if (auto CS = ImmutableCallSite(&I)) {
          const Function *CF = CS.getCalledFunction();
          if (CF != nullptr) { // TODO: Forbid indirect calls in enclaves
            if (sllvm::isEEntry(CF)) {
              if (! F.hasLocalLinkage() ) { // public functions
                EECalls.insert(&I);
              }
              if ( CF->isDeclaration() ) {
                EECalls.insert(&I);
                EXDefs.insert(CF);
              }
            }
            if (isPM()) {
              if (sllvm::isEEntry(&F) || F.hasLocalLinkage()) {
                // TODO: Treat intrinsic calls as exit calls too ? (except for
                //         the EENTER and EEXIT intrinsics of course)
                if (CF->isDeclaration() && (! CF->isIntrinsic())) {
                  EXCalls.insert(&I);
                  EXDefs.insert(CF);
                }
              }
            }
          }
        }
      }
    }
  }

  for (auto GVI = M.global_begin(), E = M.global_end(); GVI != E; GVI++) {
    if (GVI->hasLocalLinkage()) {
      // TODO: Not sure if the following line is correct for excluding
      //       string literals from the enclave data. Probably it is excluding
      //       too much. Rewrite it so that only string literals are excluded.
      if (! (GVI->isConstant() && GVI->hasAtLeastLocalUnnamedAddr()) ) {
        EData.insert(&*GVI);
      }
    }
  }
}
