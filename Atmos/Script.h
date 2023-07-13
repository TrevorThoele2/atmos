#pragma once

#include "ArcaRelicIncludes.h"

#include "ScriptData.h"
#include "FindAssetByName.h"

#include <Inscription/VectorScribe.h>

namespace Atmos::Scripting
{
    class Resource;

    class Script final
    {
    public:
        using Data = ScriptData;
    public:
        Data data;
    public:
        Script(Arca::RelicInit init, Arca::Index<Asset::Script> asset, const String& executeName, Parameters parameters);
        explicit Script(Arca::RelicInit init, Arca::Serialization);

        bool operator==(const Script& arg) const;
        bool operator!=(const Script& arg) const;
    private:
        Arca::RelicInit init;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Scripting::Script>
    {
        static constexpr ObjectType objectType = ObjectType::Relic;
        static const inline TypeName typeName = "Atmos::Scripting::Script";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Scripting::Script> final
    {
    public:
        using ObjectT = Atmos::Scripting::Script;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            format("asset", object.data.asset);
            format("executeName", object.data.executeName);
            format("parameters", object.data.parameters);
            format("isSuspended", object.data.isSuspended);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Scripting::Script, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::Script>;
    };
}