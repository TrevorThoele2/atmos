#pragma once

#include "Asset.h"

#include "Key.h"

#include <Inscription/SetScribe.h>

namespace Atmos::Asset
{
    class Action : public Asset
    {
    public:
        using MappedKeys = std::set<Input::Key>;
        MappedKeys mappedKeys;
    public:
        Action(
            Arca::RelicInit init,
            const Atmos::Name& name,
            const MappedKeys& mappedKeys);
        Action(Arca::RelicInit init, Arca::Serialization serialization);
        Action(Action&& arg) noexcept;

        Action& operator=(Action&& arg) noexcept;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::Action>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static TypeName TypeName() { return "Atmos::Asset::Action"; }
        static bool ShouldCreate(
            Reliquary& reliquary,
            const Atmos::Name& name,
            Atmos::Asset::Action::MappedKeys mappedKeys);
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Asset::Action> final
    {
    public:
        using ObjectT = Atmos::Asset::Action;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            BaseScriven<Atmos::Asset::Asset>(object, archive);
            archive("mappedKeys", object.mappedKeys);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Asset::Action, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Asset::Action>;
    };
}