#define DEBUG_TYPE "sm-creator"

#include "FunctionCcInfo.h"

#include <llvm/IR/DataLayout.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>

using namespace llvm;

FunctionCcInfo::FunctionCcInfo(Function* f)
{
    Module* m = f->getParent();

    unsigned argRegsLeft = 4;

    //for (Argument& arg : f->getArgumentList())
    Function::const_arg_iterator arg;
    for (arg = f->arg_begin(); arg != f->arg_end(); arg++) 
    {
        Type* argTy = arg->getType();

        if (arg->hasStructRetAttr())
        {
            assert(argRegsLeft == 4 && "sret not first argument?");
            retLength =
                getTypeSize(cast<PointerType>(argTy)->getElementType(), m);
            argRegsLeft--;
            continue;
        }

        if (argTy->isPointerTy() && arg->hasByValAttr())
            argTy = cast<PointerType>(argTy)->getElementType();

        unsigned argSize = getTypeSize(argTy, m);

        if (argSize % 2 == 1)
            ++argSize;

        assert(argSize != 0 && "Unsized argument?");

        if (argRegsLeft == 0 || argTy->isStructTy())
        {
            argsLength += argSize;
            continue;
        }

        // at this point we're with an integral argument and we're sure that
        // argSize is a multiple of 2. do some integrity checks
        assert(argSize != 6 && argSize <= 8 && "Impossible argument size");

        if (2 * argRegsLeft >= argSize)
            argRegsLeft -= argSize / 2;
        else
            argsLength += argSize;
    }

    switch (argRegsLeft)
    {
        case 0:
            argRegsUsage = 0x1;
            break;

        case 1:
            argRegsUsage = 0x2;
            break;

        case 2:
            argRegsUsage = 0x4;
            break;

        case 3:
            argRegsUsage = 0x8;
            break;

        case 4:
            argRegsUsage = 0x0;
            break;

        default:
            llvm_unreachable("Impossible number of registers used");
    }

    Type* retTy = f->getReturnType();

    if (!retTy->isVoidTy())
    {
        switch (getTypeSize(retTy, m))
        {
            case 8:
                retRegsUsage = 0x1;
                break;

            case 4:
                retRegsUsage = 0x2;
                break;

            case 2:
            case 1:
                retRegsUsage = 0x4;
                break;

            default:
                llvm_unreachable("Invalid return type size");
        }
    }
}

unsigned int FunctionCcInfo::getTypeSize(Type* type, const Module* m)
{
    return DataLayout(m).getTypeStoreSize(type);
}
