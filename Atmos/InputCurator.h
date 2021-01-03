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
        Delta<Spatial::ScreenPoint> mousePosition;

        [[nodiscard]] bool IsActive(std::set<Key> keys, KeyStates currentKeyStates) const;
        [[nodiscard]] std::optional<KeyState> ActionDelta(std::set<Key> keys, Delta<KeyStates> allKeyStates) const;
    private:
        struct KeyMapping
        {
            KeyState KeyStates::* toKeyStates;
        };

        std::unordered_map<Key, KeyMapping> keyMappings;

        [[nodiscard]] const KeyMapping& RequiredKeyMapping(Key key) const;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Input::Curator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "Atmos::Input::Curator";
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