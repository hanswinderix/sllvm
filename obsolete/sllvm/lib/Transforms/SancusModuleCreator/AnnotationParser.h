#ifndef ANNOTATIONPARSER_H
#define ANNOTATIONPARSER_H

#include <llvm/Pass.h>

#include <unordered_map>

namespace llvm
{
    class GlobalValue;
    class Constant;
}

struct Annotation
{
    llvm::StringRef value;
    llvm::StringRef file;
    int line;
};

llvm::raw_ostream& operator<<(llvm::raw_ostream& os, const Annotation& annot);

class AnnotationParser : public llvm::ModulePass
{
    public:

        typedef std::unordered_multimap<const llvm::GlobalValue*,
                                        Annotation> Map;

        static char ID;

        AnnotationParser();

        virtual bool runOnModule(llvm::Module& m);

        const Map& getAnnotations() const;
        std::vector<Annotation>
            getAnnotations(const llvm::GlobalValue* gv) const;

    private:

        llvm::StringRef parseString(llvm::Constant* c);

        Map annotations;
};

#endif
