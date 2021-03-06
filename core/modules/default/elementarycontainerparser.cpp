#include "elementarycontainerparser.h"

#include "core/module.h"

ElementaryContainerParser::ElementaryContainerParser(ParsingOption& option, const ObjectType &elementType, const std::string &namePattern)
    :Parser(option), elementType(elementType)
{
    if (namePattern.empty()) {
        hasFixedName = true;
        fixedName = "#";
    } else {
        nameParts = splitByChar(namePattern, '%');
        if (nameParts.size() == 1) {
            hasFixedName = true;
            fixedName = namePattern;
        } else {
            hasFixedName = false;
        }
    }
}

Object *ElementaryContainerParser::addElem()
{
    Object* child = object().addVariable(elementType);
    if (hasFixedName) {
        child->setName(fixedName);
    } else {
        const std::string indexString = toStr(object().numberOfChildren() - 1);
        const std::string name = join(nameParts, indexString);
        child->setName(name);
    }

    return child;
}

int64_t ElementaryContainerParser::getElemFixedSize() const
{
    return elementType.fixedSize();
}
