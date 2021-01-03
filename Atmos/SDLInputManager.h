#pragma once

#include "InputManager.h"

#include <SDL.h>

namespace Atmos::Input
{
    class SDLManager final : public Manager
    {
    public:
        SDLManager();

        [[nodiscard]] State ReadState() const override;
    private:
        void HandleTextInputEvent(SDL_TextInputEvent event, State& state) const;
        void HandleMouseMoveEvent(SDL_MouseMotionEvent event, State& state) const;
        template<KeyState toKeyState>
        void HandleMouseButtonEvent(SDL_MouseButtonEvent event, State& state) const;
        template<KeyState toKeyState>
        void HandleKeyboardEvent(SDL_KeyboardEvent event, State& state) const;
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

    template<KeyState toKeyState>
    void SDLManager::HandleMouseButtonEvent(SDL_MouseButtonEvent event, State& state) const
    {
        const auto keyMapping = FromMouse(event.button);
        if (keyMapping)
            state.keyStates.*keyMapping->toKeyStates = toKeyState;
    }

    template<KeyState toKeyState>
    void SDLManager::HandleKeyboardEvent(SDL_KeyboardEvent event, State& state) const
    {
        const auto keyMapping = FromKeycode(event.keysym.sym);
        if (keyMapping)
            state.keyStates.*keyMapping->toKeyStates = toKeyState;
    }
}