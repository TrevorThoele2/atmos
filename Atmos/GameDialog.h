
#pragma once

#include "InputAction.h"
#include "FrameTimer.h"

#include "Event.h"

#include <AGUI\Root.h>
#include <AGUI\Textbox.h>

namespace Atmos
{
    class StateBase;
    class GameDialog
    {
    private:
        static bool working;

        static agui::Root *root;
        static agui::Textbox *textbox;
        static agui::TextComponent *output;
        static FrameTimer timer;
        static EventBoundSubscriber inputSub;

        GameDialog() = default;
        GameDialog(const GameDialog &arg) = delete;
        GameDialog& operator=(const GameDialog &arg) = delete;

        static void OnActionPressed(const Input::Action &args);
        static void OnStateChanged(const StateBase &args);
        static void OnUsePressed();

        static void Leave();
    public:
        static void Init();
        static void Work();
        static bool Display(const std::string &str);
        static bool IsActive();
    };
}