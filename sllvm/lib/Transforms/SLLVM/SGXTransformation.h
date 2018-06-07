#ifndef SLLVM_SGX_TRANSFORMATION_H
#define SLLVM_SGX_TRANSFORMATION_H

#include "llvm/IR/Module.h"

using namespace llvm;

namespace sllvm {
  class SGXTransformation : public ModulePass {
    public:
      static char ID;

      SGXTransformation() : ModulePass(ID) {}

      bool runOnModule(Module &M) override;

    private:

      void getAnalysisUsage(AnalysisUsage &A) const override;

      Type * getStubParamType(const Function *F);

      void generateECallTable(Module &M);
      void generateDynEntryTable(Module &M);
      void generateStubs(Module &M);
      void handleOutCalls(Module &M);
  };
}

#endif
