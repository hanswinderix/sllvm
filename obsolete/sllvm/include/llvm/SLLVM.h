#ifndef SLLVM_H
#define SLLVM_H


#include "llvm/IR/Module.h"

using namespace llvm;

namespace sllvm {

  constexpr const char* module_flag_pm = "sllvm-protected-module";
  constexpr const char* attribute_eentry = "sllvm-eentry";

  inline bool isPM(const Module *M) {
    return M->getModuleFlag(module_flag_pm) != nullptr;
  }

  inline StringRef getPMName(const Module *M) {
    assert(isPM(M));
    // TODO: Is the the right way to do this ?
    return cast_or_null<MDString>(M->getModuleFlag(module_flag_pm))->getString();
  }

  inline bool isEEntry(const Function *F) {
    return F->hasFnAttribute(attribute_eentry);
  }
}

#endif
