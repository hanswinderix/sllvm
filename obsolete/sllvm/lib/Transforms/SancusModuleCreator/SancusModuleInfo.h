#ifndef SANCUSMODULEINFO_H
#define SANCUSMODULEINFO_H

#include <string>

struct SancusModuleInfo
{
    std::string name;
    bool isInSm = false;
    bool isEntry = false;
    bool isMmio = false;

    std::string getDataSection() const;
    std::string getTextSection() const;
    std::string getTableSection(const std::string& entry) const;
    std::string getStackName() const;
    std::string getSpName() const;
    std::string getEntryName() const;
    std::string getExitName() const;
    std::string getVerifyName() const;
    std::string getIndexName(const std::string& entry) const;
    std::string getCalleeIdName(const std::string& callee) const;
    std::string getCalleeMacName(const std::string& callee) const;
    std::string getCalleeStubName(const std::string& callee) const;
};

#endif
