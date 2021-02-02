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