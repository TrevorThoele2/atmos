#pragma once

#include <Chroma/NameValuePair.h>

#include "Serialization.h"

namespace Atmos
{
    using NameValuePair = Chroma::NameValuePair;
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::NameValuePair> final
    {
    public:
        using ObjectT = Atmos::NameValuePair;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            format("name", object.name);
            format("value", object.value);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::NameValuePair, Format> final
    {
        using Category = CompositeScribeCategory<Atmos::NameValuePair>;
    };
}