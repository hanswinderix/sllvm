#ifndef SLLVM_TRANSFORMATION_H
#define SLLVM_TRANSFORMATION_H

#include "llvm/IR/Module.h"

using namespace llvm;

namespace sllvm {
  class SLLVMTransformation : public ModulePass {
    public:
      static char ID;

      SLLVMTransformation() : ModulePass(ID) {}

      bool runOnModule(Module &M) override;

      void getAnalysisUsage(AnalysisUsage &A) const override;
  };
}

#endif
