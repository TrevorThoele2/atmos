#pragma once

#include <Arca/Curator.h>

#include "InputManager.h"
#include "BindAction.h"
#include "Work.h"

#include "Key.h"
#include "Delta.h"

namespace Atmos::Input
{
    class Curator final : public Arca::Curator
    {
    public:
        explicit Curator(Init init, Manager& manager);

        void Handle(const BindAction& command);
        void Handle(const Work& command);
    private:
        Manager* manager;

        Delta<KeyStates> keyStates;

        using ActionState = std::tuple<bool, std::optional<KeyState>>;
        [[nodiscard]] ActionState CurrentActionState(const Name& name, Key key, std::set<Key> modifiers, KeyStates currentKeyStates) const;
    private:
        struct KeyMapping
        {
            KeyState KeyStates::* toKeyStates;
        };

        std::unordered_map<Key, KeyMapping> keyMappings;

        [[nodiscard]] const KeyMapping& RequiredKeyMapping(Key key) const;
    private:
        using ActiveActions = std::unordered_set<Name>;
        ActiveActions previousActiveActions;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Input::Curator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static TypeName TypeName() { return "Atmos::Input::Curator"; }
        using HandledCommands = HandledCommands<
            Atmos::Input::BindAction,
            Atmos::Work>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Input::Curator, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Input::Curator>;
    };
}