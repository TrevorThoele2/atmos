
#pragma once

#include "ObjectSystem.h"
#include "ObjectReference.h"

#include "InputAction.h"
#include "FrameStopwatch.h"

#include "BoundEventSubscriber.h"

#include <AGUI/Root.h>
#include <AGUI/Textbox.h>

namespace Atmos
{
    class State;

    class GameDialogSystem : public ObjectSystem
    {
    public:
        GameDialogSystem(ObjectManager& manager);
        GameDialogSystem(const ::Inscription::Table<GameDialogSystem>& table);

        void Initialize();
        void Work();
        bool Display(const String& str);
        bool IsActive();
    private:
        bool working;

        Agui::Root* root;
        Agui::Textbox* textbox;
        Agui::TextComponent* output;
        typedef TypedObjectReference<FrameStopwatch> FrameStopwatchReference;
        FrameStopwatchReference stopwatch;
    private:
        BoundEventSubscriber inputSubscriber;
    private:
        void OnActionPressed(const Input::Action& args);
        void OnUsePressed();

        void Leave();
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::GameDialogSystem)
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
    };
}