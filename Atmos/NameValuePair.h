#pragma once

#include "Name.h"
#include "Variant.h"

#include "Serialization.h"

namespace Atmos
{
    struct NameValuePair
    {
        Name name;
        Variant value;

        NameValuePair() = default;
        NameValuePair(const Name& name, const Variant& value);
        NameValuePair(Name&& name, Variant&& value);
        NameValuePair(const NameValuePair& arg);
        NameValuePair(NameValuePair&& arg) noexcept;

        NameValuePair& operator=(const NameValuePair& arg);
        NameValuePair& operator=(NameValuePair&& arg) noexcept;

        [[nodiscard]] bool operator==(const NameValuePair& arg) const;
        [[nodiscard]] bool operator!=(const NameValuePair& arg) const;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::NameValuePair> final
    {
    public:
        using ObjectT = Atmos::NameValuePair;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            archive("name", object.name);
            archive("value", object.value);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::NameValuePair, Archive> final
    {
        using Category = CompositeScribeCategory<Atmos::NameValuePair>;
    };
}