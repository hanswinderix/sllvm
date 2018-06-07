#include "Transformation.h"
#include "Analysis.h"
#include "llvm/SLLVM.h"

#define DEBUG_TYPE "sllvm"

using namespace sllvm;

bool SLLVMTransformation::runOnModule(Module &M) {
  bool result = false;

  if (getAnalysis<SLLVMAnalysis>().getResults().isPM()) {
    // TODO: Come up with a better module name (see todo list)
    auto N = llvm::MDString::get(M.getContext(), M.getSourceFileName());
    // TODO: What happens when llvm-linking (which name is taken?)
    M.addModuleFlag(Module::Override, sllvm::module_flag_pm, N);
    result = true;
  }

  return result;
}

char SLLVMTransformation::ID = 0;

static RegisterPass<SLLVMTransformation> X(
    "sllvm-transform", "SLLVM transformation pass");

void SLLVMTransformation::getAnalysisUsage(AnalysisUsage &A) const {
  A.addRequired<SLLVMAnalysis>();
}
