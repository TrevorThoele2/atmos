#pragma once

#include "ArcaRelicIncludes.h"

#include "ScriptAsset.h"
#include "ScriptParameters.h"
#include "ScriptResource.h"
#include "FindAssetByName.h"

#include <Inscription/VectorScribe.h>

namespace Atmos::Scripting
{
    class Resource;

    class Script final
    {
    public:
        using ResourceT = Resource;
        using ResourcePtr = std::unique_ptr<ResourceT>;
    public:
        Arca::Index<Asset::Script> asset;
        String executeName;
        Parameters parameters;

        bool isSuspended = false;
    public:
        Script(Arca::RelicInit init, Arca::Index<Asset::Script> asset, const String& executeName, Parameters parameters);
        explicit Script(Arca::RelicInit init, Arca::Serialization);

        bool operator==(const Script& arg) const;
        bool operator!=(const Script& arg) const;

        [[nodiscard]] Resource* Resource() const;
        template<class ResourceT>
        [[nodiscard]] ResourceT* Resource() const;
    public:
        void Setup(ResourcePtr&& set);
    private:
        ResourcePtr resource;
    private:
        Arca::RelicInit init;
    private:
        INSCRIPTION_ACCESS;
    };

    template<class ResourceT>
    ResourceT* Script::Resource() const
    {
        return static_cast<ResourceT*>(resource.get());
    }
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Scripting::Script>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static TypeName TypeName() { return "Atmos::Scripting::Script"; }
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
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            archive("asset", object.asset);
            archive("executeName", object.executeName);
            archive("parameters", object.parameters);
            archive("isSuspended", object.isSuspended);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Scripting::Script, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::Script>;
    };
}