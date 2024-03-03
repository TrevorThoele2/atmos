
#include "NameValuePair.h"

namespace Atmos
{
    NameValuePair::NameValuePair(const Name &name, const Variant &value) : name(name), value(value)
    {}

    NameValuePair::NameValuePair(Name &&name, Variant &&value) : name(std::move(name)), value(std::move(value))
    {}

    NameValuePair::NameValuePair(NameValuePair &&arg) : name(std::move(arg.name)), value(std::move(arg.value))
    {}

    NameValuePair& NameValuePair::operator=(NameValuePair &&arg)
    {
        name = std::move(arg.name);
        value = std::move(arg.value);
        return *this;
    }

    bool NameValuePair::operator==(const NameValuePair &arg) const
    {
        return name == arg.name && value == arg.value;
    }

    bool NameValuePair::operator!=(const NameValuePair &arg) const
    {
        return !(*this == arg);
    }

    const Name& NameValuePair::GetName() const
    {
        return name;
    }

    const Variant& NameValuePair::GetValue() const
    {
        return value;
    }
}