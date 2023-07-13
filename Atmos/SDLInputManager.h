#pragma once

#include "InputManager.h"

#include "SDLIncludes.h"

namespace Atmos::Input
{
    class SDLManager final : public Manager
    {
    public:
        explicit SDLManager(Logging::Logger& logger);

        [[nodiscard]] State ReadState() const override;
    private:
        void HandleTextInputEvent(SDL_TextInputEvent event, State& state) const;
        void HandleMouse(State& state) const;
        void HandleKeyboard(State& state) const;
    private:
        struct KeyMapping
        {
            KeyState KeyStates::* toKeyStates;
        };

        std::unordered_map<int, KeyMapping> mouseButtonMappings;
        std::unordered_map<SDL_Keycode, KeyMapping> keycodeMappings;

        [[nodiscard]] const KeyMapping* FromMouse(int mouseButton) const;
        [[nodiscard]] const KeyMapping* FromKeycode(SDL_Keycode keycode) const;
    };
}