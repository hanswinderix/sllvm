#ifndef FUNCTIONCCINFO_H
#define FUNCTIONCCINFO_H

#include <string>

namespace llvm
{
    class Function;
    class Module;
    class Type;
}

class FunctionCcInfo
{
    public:
        FunctionCcInfo(llvm::Function* f);

        // number of bytes of stack arguments
        unsigned argsLength = 0;

        // 0x0: none
        // 0x1: r12:r13:r14:r15
        // 0x2: r13:r14:r15
        // 0x4: r14:r15
        // 0x8: r15
        unsigned argRegsUsage = 0;

        // number of bytes of stack reserved space for return value
        unsigned retLength = 0;

        // 0x0: none
        // 0x1: r15:r14:r13:r12
        // 0x2: r15:r14
        // 0x4: r15
        unsigned retRegsUsage = 0;

    private:

        static unsigned getTypeSize(llvm::Type* type, const llvm::Module* m);
};

#endif
