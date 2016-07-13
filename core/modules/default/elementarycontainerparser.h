#ifndef ELEMENTARYCONTAINERPARSER_H
#define ELEMENTARYCONTAINERPARSER_H

#include "core/parser.h"

/**
 * @brief Parent class for both ArrayParser and Tuple parser
 */
class ElementaryContainerParser : public Parser
{
public:
    ElementaryContainerParser(ParsingOption& option, const ObjectType& elementType, const std::string &namePattern);

protected:
    Object* addElem();
    int64_t getElemFixedSize() const;

private:
    ObjectType elementType;
    bool hasFixedName;
    std::string fixedName;
    std::vector<std::string> nameParts;
};

#endif // ELEMENTARYCONTAINERPARSER_H
