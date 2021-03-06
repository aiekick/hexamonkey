#ifndef LOCALSCOPE_H
#define LOCALSCOPE_H

#include <unordered_map>
#include <string>

#include "core/variable/variable.h"

class Module;

class LocalScope : public VariableImplementation
{
public:
    LocalScope(const Variable& context, const Module& module);

    virtual void collect(const VariableAdder &addAccessible) override;
protected:
    virtual Variable doGetField(const Variant &key, bool modifiable, bool createIfNeeded) override;
    virtual void doSetField(const Variant &key, const Variable &variable) override;
    virtual void doRemoveField(const Variant &key) override;

private:
    std::unordered_map<std::string, VariableMemory> _fields;
    VariableMemory _context;
    const Module& _module;
};

#endif // LOCALSCOPE_H
