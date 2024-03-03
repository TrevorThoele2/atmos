#pragma once

#include "Asset.h"

#include "Key.h"

#include <Inscription/SetScribe.h>

namespace Atmos::Asset
{
    class Action : public Asset
    {
    public:
        Input::Key boundKey = {};

        using Modifiers = std::set<Input::Key>;
        Modifiers boundModifiers;
    public:
        Action(
            Arca::RelicInit init,
            const Atmos::Name& name,
            Input::Key boundKey,
            const Modifiers& boundModifiers);
        Action(Arca::RelicInit init, Arca::Serialization serialization);
        Action(Action&& arg) noexcept;

        Action& operator=(Action&& arg) noexcept;

        bool operator==(const Action& arg) const;
        bool operator!=(const Action& arg) const;
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
            Atmos::Input::Key boundKey,
            const Atmos::Asset::Action::Modifiers& boundModifiers);
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
            archive("boundKey", object.boundKey);
            archive("boundModifiers", object.boundModifiers);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Asset::Action, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Asset::Action>;
    };
}