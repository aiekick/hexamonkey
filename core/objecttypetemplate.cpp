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

#include "objecttypetemplate.h"
#include "objecttype.h"

ObjectTypeTemplate::ObjectTypeTemplate(const std::string &name, const std::vector<std::string> &parameterNames)
    :_name(name), _parametersNames(parameterNames)

{
    for(unsigned int i = 0; i < _parametersNames.size(); ++i)
    {
        _parametersNumbers[_parametersNames[i]] = i;
    }
}

ObjectTypeTemplate::ObjectTypeTemplate(const std::string& name)
    : ObjectTypeTemplate(name, std::vector<std::string>())
{
}


const std::string& ObjectTypeTemplate::name() const
{
    return _name;
}

int ObjectTypeTemplate::numberOfParameters() const
{
    return _parametersNames.size();
}

const std::string& ObjectTypeTemplate::parameterName(int index) const
{
    return _parametersNames.at(index);
}

int ObjectTypeTemplate::parameterNumber(const std::string &name) const
{
    const auto it = _parametersNumbers.find(name);
    if(it != _parametersNumbers.end())
        return it->second;
    return -1;
}

bool ObjectTypeTemplate::isNull() const
{
    return _name.empty();
}

void ObjectTypeTemplate::addParameter(const std::string& parameterName)
{
    _parametersNames.push_back(parameterName);
}

bool operator==(const ObjectTypeTemplate& a, const ObjectTypeTemplate& b)
{
    return (a._name == b._name);
}

bool operator!=(const ObjectTypeTemplate& a, const ObjectTypeTemplate& b)
{
    return !(a == b);
}

bool operator< (const ObjectTypeTemplate& a, const ObjectTypeTemplate& b)
{
    return (a._name < b._name);
}

bool operator<= (const ObjectTypeTemplate& a, const ObjectTypeTemplate& b)
{
    return (a<b)||(a==b);
}