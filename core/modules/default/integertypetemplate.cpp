#include "integertypetemplate.h"
#include "core/object.h"
#include "core/parsingexception.h"

IntegerTypeTemplate::IntegerTypeTemplate()
    : ObjectTypeTemplate("int",{"size", "_base"})
{
}

Parser *IntegerTypeTemplate::parseOrGetParser(const ObjectType &type, ParsingOption &option, const Module&) const
{
    Object::ParsingContext context(option);

    const int64_t size =  type.parameterValue(0).toInteger();
    Object& object = context.object();

    object.setSize(size);

    Variant value;

    switch (size) {
    case 8:
    {
        int8_t integer;
        object.file().read(reinterpret_cast<char* >(&integer), 8);
        value.setValue(integer);
        break;
    }

    case 16:
    {
        int16_t integer;
        if(object.endianness() == Object::bigEndian)
        {
            object.file().read(reinterpret_cast<char* >(&integer)+1, 8);
            object.file().read(reinterpret_cast<char* >(&integer), 8);
        } else {
            object.file().read(reinterpret_cast<char* >(&integer), 16);
        }
        value.setValue(integer);
        break;
    }

    case 32:
    {
        int32_t integer;
        object.file().read(reinterpret_cast<char* >(&integer), 32);
        if(object.endianness() == Object::bigEndian) {
            integer = __builtin_bswap32(integer);
        }

        value.setValue(integer);
        break;
    }

    case 64:
    {
        int64_t integer;
        object.file().read(reinterpret_cast<char* >(&integer), 64);
        if(object.endianness() == Object::bigEndian) {
            integer = __builtin_bswap64(integer);
        }

        value.setValue(integer);
        break;
    }

    default:
        if(size>64)
        {
            throw ParsingException(ParsingException::Type::BadParameter, "Integer size must be lower than 64");
        }
        {
            int byteSize = (size + 7) >> 3;
            char buffer[byteSize];
            int64_t integer = 0;
            char* pInteger = reinterpret_cast<char* >(&integer);
            object.file().read(buffer, size);
            for (int i = 0 ; i < byteSize ; i++) {
                if(object.endianness() == Object::bigEndian)
                    pInteger[byteSize - 1 - i] = buffer[i];
                else
                    pInteger[i] = buffer[i];
            }

            if (integer & 1LL<<(size-1) ) {
                integer |= 0xFFFFFFFFFFFFFFFFLL << size;
            }

            value.setValue(integer);
        }
        break;
    }



    int base = 0;
    if (type.parameterSpecified(1)) {
        base = type.parameterValue(1).toInteger();
    }
    switch (base) {
        case 2:
            value.setDisplayBase(Variant::binary);
            break;

        case 8:
            value.setDisplayBase(Variant::octal);
            break;

        case 16:
            value.setDisplayBase(Variant::hexadecimal);
            break;

        default:
            value.setDisplayBase(Variant::decimal);
    }

    object.setValue(value);

    return nullptr;
}

int64_t IntegerTypeTemplate::fixedSize(const ObjectType &objectType, const Module&) const
{
    return objectType.parameterValue(0).toInteger();
}


UIntegerTypeTemplate::UIntegerTypeTemplate()
    : ObjectTypeTemplate("uint",{"size", "_base"})
{
}

Parser *UIntegerTypeTemplate::parseOrGetParser(const ObjectType &type, ParsingOption &option, const Module&) const
{
    Object::ParsingContext context(option);

    const int64_t size =  type.parameterValue(0).toInteger();
    Object& object = context.object();

    object.setSize(size);

    Variant value;

    switch (size) {
    case 8:
    {
        uint8_t integer;
        object.file().read(reinterpret_cast<char* >(&integer), 8);
        value.setValue(integer);
        break;
    }

    case 16:
    {
        uint16_t integer;
        if(object.endianness() == Object::bigEndian)
        {
            object.file().read(reinterpret_cast<char* >(&integer)+1, 8);
            object.file().read(reinterpret_cast<char* >(&integer), 8);
        } else {
            object.file().read(reinterpret_cast<char* >(&integer), 16);
        }
        value.setValue(integer);
        break;
    }

    case 32:
    {
        uint32_t integer;
        object.file().read(reinterpret_cast<char* >(&integer), 32);
        if(object.endianness() == Object::bigEndian) {
            integer = __builtin_bswap32(integer);
        }

        value.setValue(integer);
        break;
    }

    case 64:
    {
        uint64_t integer;
        object.file().read(reinterpret_cast<char* >(&integer), 64);
        if(object.endianness() == Object::bigEndian) {
            integer = __builtin_bswap64(integer);
        }

        value.setValue(integer);
        break;
    }

    default:
        if(size>64)
        {
            throw ParsingException(ParsingException::Type::BadParameter, "Integer size must be lower than 64");
        }
        {
            int byteSize = (size + 7) >> 3;
            char buffer[byteSize];
            uint64_t integer = 0;
            char* pInteger = reinterpret_cast<char* >(&integer);
            object.file().read(buffer, size);
            for (int i = 0 ; i < byteSize ; i++) {
                if(object.endianness() == Object::bigEndian)
                    pInteger[byteSize - 1 - i] = buffer[i];
                else
                    pInteger[i] = buffer[i];
            }

            value.setValue(integer);
        }
        break;
    }



    int base = 0;
    if (type.parameterSpecified(1)) {
        base = type.parameterValue(1).toInteger();
    }
    switch (base) {
        case 2:
            value.setDisplayBase(Variant::binary);
            break;

        case 8:
            value.setDisplayBase(Variant::octal);
            break;

        case 16:
            value.setDisplayBase(Variant::hexadecimal);
            break;

        default:
            value.setDisplayBase(Variant::decimal);
    }

    object.setValue(value);

    return nullptr;
}

int64_t UIntegerTypeTemplate::fixedSize(const ObjectType &objectType, const Module&) const
{
    return objectType.parameterValue(0).toInteger();
}



ByteTypeTemplate::ByteTypeTemplate()
    : ObjectTypeTemplate("byte")
{
}

Parser *ByteTypeTemplate::parseOrGetParser(const ObjectType &/*type*/, ParsingOption &option, const Module&) const
{
    Object::ParsingContext context(option);

    Object& object = context.object();

    object.setSize(8);

    Variant value;

    uint8_t integer;
    object.file().read(reinterpret_cast<char* >(&integer), 8);
    value.setValue(integer);
    value.setDisplayBase(Variant::hexadecimal);

    object.setValue(value);

    return nullptr;
}

int64_t ByteTypeTemplate::fixedSize(const ObjectType &/*objectType*/, const Module&) const
{
    return 8;
}


UuidTypeTemplate::UuidTypeTemplate()
    : ObjectTypeTemplate("uuid")
{
}

Parser *UuidTypeTemplate::parseOrGetParser(const ObjectType &/*objectType*/, ParsingOption &option, const Module&) const
{
    Object::ParsingContext context(option);

    Object& object = context.object();

    object.setSize(128);

    std::stringstream S;

    int32_t i0;
    int16_t i1;
    int16_t i2;
    int64_t i3;

    if(object.endianness() == Object::bigEndian) {
        object.file().read(reinterpret_cast<char* >(&i0), 32);
        i0 = __builtin_bswap32(i0);

        object.file().read(reinterpret_cast<char* >(&i1)+1, 8);
        object.file().read(reinterpret_cast<char* >(&i1), 8);

        object.file().read(reinterpret_cast<char* >(&i2)+1, 8);
        object.file().read(reinterpret_cast<char* >(&i2), 8);
    } else {
        object.file().read(reinterpret_cast<char* >(&i0), 32);
        object.file().read(reinterpret_cast<char* >(&i1), 16);
        object.file().read(reinterpret_cast<char* >(&i2), 16);
    }
    //always big endian
    object.file().read(reinterpret_cast<char* >(&i3), 64);
    i3 = __builtin_bswap64(i3);

    S<<std::hex<<std::setfill('0')<<std::uppercase
     <<std::setw(8)<<i0<<"-"
     <<std::setw(4)<<i1<<"-"
     <<std::setw(4)<<i2<<"-"
     <<std::setw(4)<<((i3>>48)&0xffff)<<"-"
     <<std::setw(12)<<(i3&0xffffffffffffLL);

    object.setValue(S.str());

    return nullptr;
}

int64_t UuidTypeTemplate::fixedSize(const ObjectType &/*objectType*/, const Module&) const
{
    return 128;
}