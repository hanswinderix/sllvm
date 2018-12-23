#include "Transformation.h"
#include "Analysis.h"
#include "llvm/SLLVM.h"

#define DEBUG_TYPE "sllvm"

using namespace sllvm;

bool SLLVMTransformation::runOnModule(Module &M) {
  bool result = false;

  if (getAnalysis<SLLVMAnalysis>().getResults().isPM()) {
    auto FN = M.getSourceFileName();

    // Strip file extensions
    auto idx = FN.find_first_of(".");
    if (idx != std::string::npos) {
      FN = FN.substr(0, idx);
    }

    std::replace(FN.begin(), FN.end(), '-', '_');

    auto N = llvm::MDString::get(M.getContext(), FN);

    // TODO: Come up with a better module name (see todo list)
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
