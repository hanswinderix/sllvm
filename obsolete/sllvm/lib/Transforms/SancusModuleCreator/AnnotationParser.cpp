#include "AnnotationParser.h"

#include <llvm/IR/Module.h>
#include <llvm/IR/Constants.h>

#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/Debug.h>

using namespace llvm;

raw_ostream& operator<<(raw_ostream& os, const Annotation& annot)
{
    return os << "Annotation(" << annot.value << ", " << annot.file << ", "
              << annot.line << ")";
}

AnnotationParser::AnnotationParser() : ModulePass(ID)
{
}

bool AnnotationParser::runOnModule(llvm::Module& m)
{
    GlobalVariable* gv = m.getGlobalVariable("llvm.global.annotations");

    if (gv == nullptr)
        return false;

    ConstantArray* arr = cast<ConstantArray>(gv->getOperand(0));

    for (ConstantArray::op_iterator it = arr->op_begin(), end = arr->op_end();
         it != end; ++it)
    {
        ConstantStruct* annotStruct = cast<ConstantStruct>(it);
        assert(annotStruct->getNumOperands() == 4);

        Annotation annotation;
        Constant* gvOp = annotStruct->getOperand(0);
        GlobalValue* key;

        if (ConstantExpr* ce = dyn_cast<ConstantExpr>(gvOp))
        {
            // sanity checks
            assert(ce->isCast() || ce->isGEPWithNoNotionalOverIndexing());
            (void)ce; // remove unused variable warnings in release build

            key = cast<GlobalValue>(gvOp->getOperand(0));
        }
        else
            key = cast<GlobalValue>(gvOp);

        annotation.value = parseString(annotStruct->getOperand(1));
        annotation.file = parseString(annotStruct->getOperand(2));
        annotation.line =
            cast<ConstantInt>(annotStruct->getOperand(3))->getSExtValue();

        annotations.insert({key, annotation});

//         dbgs() << "Annotation for " << key->getName() << ":\n";
//         dbgs() << " - Value: " << annotation.value << "\n";
//         dbgs() << " - File:  " << annotation.file << "\n";
//         dbgs() << " - Line:  " << annotation.line << "\n";
    }

    return false;
}

const AnnotationParser::Map& AnnotationParser::getAnnotations() const
{
    return annotations;
}

std::vector<Annotation>
AnnotationParser::getAnnotations(const llvm::GlobalValue* gv) const
{
    auto range = annotations.equal_range(gv);
    std::vector<Annotation> ret;

    std::transform(range.first, range.second, std::back_inserter(ret),
                   [](decltype(*range.first)& v) {return v.second;});

    return ret;
}

StringRef AnnotationParser::parseString(Constant* c)
{
    assert(cast<ConstantExpr>(c)->isGEPWithNoNotionalOverIndexing());

    GlobalVariable* gv = cast<GlobalVariable>(c->getOperand(0));
    return cast<ConstantDataSequential>(gv->getOperand(0))->getAsCString();
}

char AnnotationParser::ID = 0;
static RegisterPass<AnnotationParser> AP("parse-annotations",
                                         "Parse annotations");
