#pragma once

#include <Arca/Relic.h>

#include "InputID.h"
#include "Volume.h"

namespace Atmos::Initialization
{
    class Information final
    {
    public:
        struct Graphics
        {
            bool fullscreen;
        } graphics{};
    public:
        struct Sound
        {
            Audio::Volume masterVolume;
        } sound{};
    public:
        struct Controls
        {
            struct Actions
            {
                Input::InputID moveLeft;
                Input::InputID moveUp;
                Input::InputID moveRight;
                Input::InputID moveDown;
                Input::InputID use;
                Input::InputID menuLeft;
                Input::InputID menuUp;
                Input::InputID menuRight;
                Input::InputID menuDown;
                Input::InputID selectPreviousCharacter;
                Input::InputID selectNextCharacter;
                Input::InputID endTurn;
                Input::InputID attack;
                Input::InputID cancel;
                Input::InputID inventory;
                Input::InputID spells;
                Input::InputID stats;
            } actions;
        } controls{};
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Initialization::Information>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::Initialization::Information";
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Initialization::Information, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Initialization::Information>;
    };
}