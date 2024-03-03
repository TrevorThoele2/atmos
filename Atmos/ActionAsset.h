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
        static constexpr ObjectType objectType = ObjectType::Relic;
        static const inline TypeName typeName = "Atmos::Asset::Action";
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
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            BaseScriven<Atmos::Asset::Asset>(object, format);
            format("boundKey", object.boundKey);
            format("boundModifiers", object.boundModifiers);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Asset::Action, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Asset::Action>;
    };
}