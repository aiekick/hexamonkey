#include "enumtypetemplate.h"

#include "core/object.h"
#include "core/parser.h"
#include "core/objecttype.h"
#include "core/parsingexception.h"
#include "core/variable/objectattributes.h"
#include "core/module.h"

EnumTypeTemplate::EnumTypeTemplate()
    : ObjectTypeTemplate("Enum", {"type"})
{
}

Parser *EnumTypeTemplate::parseOrGetParser(const ObjectType &type, ParsingOption &option) const
{
    if (!type.parameterSpecified(0)) {
        throw ParsingException(ParsingException::Type::BadParameter, "No enum type given");
    }

    Object::ParsingContext context(option);

    std::unique_ptr<Object> childPtr(context.object().readVariable(type.parameterValue(0).toObjectType()));
    Object& child = *childPtr;

    const Variant& value = child.value();
    for (int i = 0, n = (type.numberOfParameters()-1)/2; i < n; ++i) {
        if (value == type.parameterValue(2*i+1)) {
            context.object().attributes(true)->addNumbered().setValue(type.parameterValue(2*i+2));
        }
    }

    context.object().setValue(value);
    context.object().setSize(child.size());

    return nullptr;
}

int64_t EnumTypeTemplate::fixedSize(const ObjectType &type) const
{
    if (type.parameterSpecified(0)) {
        const ObjectType& childType = type.parameterValue(0).toObjectType();
        const int64_t t = childType.fixedSize();
        return t;
    } else {
        return -1;
    }
}

Variant EnumTypeTemplate::attributeValue(const ObjectType &type, ObjectTypeTemplate::Attribute attribute) const
{
    if (attribute == ObjectTypeTemplate::Attribute::displayAs) {
        return type.parameterValue(0);
    } else {
        return Variant();
    }
}
