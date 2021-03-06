//This file is part of the HexaMonkey project, a multimedia analyser
//Copyright (C) 2013  Sevan Drapeau-Martin, Nicolas Fleury

//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or (at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef VARIABLE_H
#define VARIABLE_H

#include <memory>
#include <vector>
#include <functional>
#include <unordered_map>

#include "core/variant.h"
#include "core/variable/variablepath.h"

#define variableLambda (const VariableArgs& args, const VariableKeywordArgs& kwargs) ->Variable

class Variable;
class VariableImplementation;
class VariableCollector;
class VariableMemory;

typedef std::vector<Variable> VariableArgs;
typedef std::unordered_map<std::string, Variable> VariableKeywordArgs;
typedef std::function<Variable(const VariableArgs&, const VariableKeywordArgs&)> VariableLambda;
typedef std::function<void (VariableMemory &)> VariableAdder;

union VariableTag {
    VariableTag() : data(0) {}
    VariableTag(int32_t data) : data(data) {}
    VariableTag(bool modifiable)
        : flags(modifiable) {}

    int32_t data;
    struct Flags {
        Flags(bool modifiable)
            : defined(1),
              modifiable(modifiable) {
        }

        int8_t defined;
        int8_t modifiable;
        int16_t padding;
    } flags;
};

/**
 * @brief Accesser (getter and setter) for a value
 *
 * The variable can also be constant or modifiable. When the variable
 * is constant and the setter is used then the variable will be transformed
 * into a copy of the current value
 *
 * A variable owns a \link VariableImplementation variable implementation\endlink that actually define
 * the getter and setter for the \link Variable variable\endlink. Common implementations can be used via
 * the static constructors: copy, ref, constRef, null and nullConstant.
 */
class Variable
{
public:
    /**
     * @brief Construct an undefined \link Variable variable\endlink.
     */
    Variable();
    /**
     * @brief Construct a variable
     * @param implementation Must be a new created instance of a subclass of \link VariableImplementation variable implementation\endlink
     * @param modifiable
     */
    Variable(VariableImplementation* implementation, bool modifiable);

    Variable(const Variable& variable);
    Variable(Variable&& variable);
    Variable(const VariableMemory& variable);
    ~Variable();
    Variable& operator=(const Variable& rhs);
    Variable& operator=(Variable&& rhs);

    /**
     * @brief Set the value of the \link Variable variable\endlink
     */
    void setValue(const Variant &value) const;

    /**
     * @brief  Get the value of the \link Variable variable\endlink
     */
    Variant value() const;

    /**
     * @brief Get field by key
     * @param key
     * @param modifable Set to true if the returned variable will used as left-value
     */
    Variable field(const Variant& key, bool modifiable = false, bool createIfNeeded = false) const;

    /**
     * @brief Get field by path
     * @param path
     * @param modifable Set to true if the returned variable will used as left-value
     */
    Variable field(const VariablePath &path, bool modifable = false, bool createIfNeeded = false) const;

    /**
     * @brief Set field by key
     */
    void setField(const Variant& key, const Variable& variable) const;

    /**
     * @brief Set field by path
     */
    void setField(const VariablePath &path, const Variable& variable) const;

    /**
     * @brief Remove field by key
     */
    void removeField(const Variant &key) const;

    /**
     * @brief Remove field by path
     */
    void removeField(const VariablePath &path) const;


    /**
     * @brief call as function
     */
    Variable call(VariableArgs& args, VariableKeywordArgs& kwargs) const;

    /**
     * @brief Set the variable as constant which prevents modification of the fields
     */
    inline void setConstant()
    {
        _tag.flags.modifiable = false;
    }

    /**
     * @brief Check if the variable not been constructed by the default constructor.
     */
    inline bool isDefined() const
    {
        return _tag.flags.defined;
    }



    VariableCollector& collector() const;

    friend void swap(Variable &first, Variable &second);
private:
    friend class VariableMemory;
    friend class VariableCollector;

    enum class Error {constModification};

    VariableImplementation* _implementation;
    VariableTag _tag;
};

void swap(Variable &first, Variable &second);

class VariableMemory
{
public:
    VariableMemory();
    VariableMemory(const Variable& variable);
    VariableMemory(const VariableMemory& variable) = default;

private:
    friend class Variable;
    friend class VariableCollector;

    VariableMemory(VariableImplementation* implementation); /*=>*/ friend class VariableImplementation;

    VariableImplementation* _implementation;
    VariableTag _tag;

};

/**
 * @brief Actual implementation for variable
 */
class VariableImplementation
{
public:
    VariableImplementation(VariableCollector& variableCollector);
    virtual ~VariableImplementation();
    virtual void collect(const std::function<void(VariableMemory&)>& addAccessible);


protected:    
    inline VariableCollector& collector() const {
        return *_collector;
    }
    friend class Variable;

    VariableMemory memory();

    virtual Variant doGetValue();
    virtual void doSetValue(const Variant& value);

    virtual Variable doGetField(const Variant& key, bool modifiable, bool createIfNeeded);
    virtual void doSetField(const Variant& key, const Variable& variable);
    virtual void doRemoveField(const Variant& key);

    virtual Variable doCall(VariableArgs &args, VariableKeywordArgs &kwargs);
private:
    VariableImplementation(); /* <--- */ friend class UndefinedVariableImplementation;

    mutable VariableCollector* _collector;
};

#endif // VARIABLE_H
