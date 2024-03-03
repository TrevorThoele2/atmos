#pragma once

#include "Name.h"
#include "Variant.h"

namespace Atmos
{
    class NameValuePair
    {
    private:
        INSCRIPTION_ACCESS;
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
    public:
        Name name;
        Variant value;

        NameValuePair() = default;
        NameValuePair(const Name &name, const Variant &value);
        NameValuePair(Name &&name, Variant &&value);
        NameValuePair(const NameValuePair &arg) = default;
        NameValuePair(NameValuePair &&arg);
        NameValuePair& operator=(const NameValuePair &arg) = default;
        NameValuePair& operator=(NameValuePair &&arg);
        bool operator==(const NameValuePair &arg) const;
        bool operator!=(const NameValuePair &arg) const;

        const Name& GetName() const;
        const Variant& GetValue() const;
    };
}