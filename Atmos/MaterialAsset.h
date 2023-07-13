#pragma once

#include "Asset.h"

#include "ScriptData.h"

#include <Inscription/VectorScribe.h>

namespace Atmos::Asset
{
    class Material final : public Asset
    {
    public:
        Scripting::ScriptData scriptData;
    public:
        Material(
            Arca::RelicInit init,
            const Atmos::Name& name,
            const Arca::Index<Script>& asset,
            const String& executeName,
            const Scripting::Parameters& parameters);
        Material(Arca::RelicInit init, Arca::Serialization serialization);
        Material(Material&& arg) noexcept;

        Material& operator=(Material&& arg) noexcept;

        bool operator==(const Material& arg) const;
        bool operator!=(const Material& arg) const;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::Material>
    {
        static constexpr ObjectType objectType = ObjectType::Relic;
        static const inline TypeName typeName = "Atmos::Asset::Material";
        static bool ShouldCreate(
            Reliquary& reliquary,
            const Atmos::Name& name,
            const Index<Atmos::Asset::Script>& asset,
            const Atmos::String& executeName,
            const Atmos::Scripting::Parameters& parameters);
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Asset::Material> final
    {
    public:
        using ObjectT = Atmos::Asset::Material;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            BaseScriven<Atmos::Asset::Asset>(object, format);
            format("asset", object.scriptData.asset);
            format("executeName", object.scriptData.executeName);
            format("parameters", object.scriptData.parameters);
            format("isSuspended", object.scriptData.isSuspended);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Asset::Material, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Asset::Material>;
    };
}