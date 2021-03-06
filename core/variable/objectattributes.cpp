#include "objectattributes.h"

#include "core/log/logmanager.h"
#include "core/variable/variablecollector.h"

ObjectAttributes::ObjectAttributes(VariableCollector &collector)
    : VariableImplementation(collector)
{
}

Variant& ObjectAttributes::addNumbered()
{
    _numberedFields.push_back(Variant::null());
    return _numberedFields.back();
}

Variant &ObjectAttributes::getNumbered(size_t number)
{
    return _numberedFields[number];
}

const Variant &ObjectAttributes::getNumbered(size_t number) const
{
    return _numberedFields[number];
}

size_t ObjectAttributes::numberedCount() const
{
    return _numberedFields.size();
}


Variant* ObjectAttributes::addNamed(const std::string &name)
{
    _fieldNames.push_back(name);
    auto inserted = _namedFields.insert(std::make_pair(name, Variant::null()));

    if (inserted.second) {
        return &inserted.first->second;
    } else {
        Log::error("Pushing existing attribute of existing name");
        return nullptr;
    }
}

Variant *ObjectAttributes::getNamed(const std::string &name)
{
    auto it = _namedFields.find(name);
    if (it != _namedFields.end()) {
        return &it->second;
    } else {
        return nullptr;
    }
}

const Variant *ObjectAttributes::getNamed(const std::string &name) const
{
    const auto it = _namedFields.find(name);
    if (it != _namedFields.cend()) {
        return &it->second;
    } else {
        return nullptr;
    }
}

const std::vector<std::string> &ObjectAttributes::fieldNames() const
{
    return _fieldNames;
}

Variable ObjectAttributes::doGetField(const Variant &key, bool /*modifiable*/, bool createIfNeeded)
{
    if (key.isValueless()) {
        if (createIfNeeded) {
            return collector().ref(addNumbered());
        } else {
            if (_numberedFields.size() > 0) {
                return collector().constRef(_numberedFields.back());
            } else {
                return Variable();
            }
        }
    } else if (key.hasNumericalType()) {
        int64_t number = key.toInteger();
        if (number >= 0) {
            if (createIfNeeded) {
                while (number >= _numberedFields.size()) {
                    addNumbered();
                }
                return collector().ref(_numberedFields[number]);
            } else if (number < _numberedFields.size()) {
                return collector().ref(_numberedFields[number]);
            } else {
                return Variable();
            }
        } else {
            if (createIfNeeded) {
                Log::error("Trying to assign a numbered attribute value with a negative value");
            }
            return Variable();
        }
    } else if (key.type() == Variant::stringType) {
        const std::string& name = key.toString();
        auto it = _namedFields.find(name);
        if (it != _namedFields.end()) {
            return collector().ref(it->second);
        } else if (createIfNeeded){
            return collector().refIfNotNull(addNamed(name));
        } else {
            return Variable();
        }
    } else {
        if (createIfNeeded) {
            Log::error("Trying to assign an attribute with an invalid key type");
        }
        return Variable();
    }
}
