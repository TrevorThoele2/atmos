
#pragma once

#include "InputKeyID.h"
#include "InputMouseKeyID.h"
#include "InputActionID.h"

#include "Name.h"

#include <AGUI/Input.h>
#include <Chroma/Enum.h>

namespace Atmos
{
    namespace Input
    {
        typedef ::Chroma::EnumIterationTraits<KeyID, KeyID::A, KeyID::L_ALT> KeyIterationTraits;
        typedef ::Chroma::EnumIterationTraits<MouseKeyID, MouseKeyID::LEFT, MouseKeyID::RIGHT> MouseIterationTraits;
        typedef ::Chroma::EnumIterationTraits<ActionID, ActionID::MOVE_UP, ActionID::STATS> ActionIterationTraits;

        template<KeyID id>
        struct KeyTraits;

#define KEY_TRAITS_CREATOR(idPass, aguiIDPass, canUseForActionPass) \
template<>                                                          \
struct KeyTraits<idPass>                                            \
{                                                                   \
    static constexpr KeyID id = idPass;                             \
    static constexpr ::Agui::KeyID aguiID = aguiIDPass;             \
    static const Name displayName;                                  \
    static constexpr bool canUseForAction = canUseForActionPass;    \
}

        KEY_TRAITS_CREATOR(KeyID::A, ::Agui::KeyID::A, true);
        KEY_TRAITS_CREATOR(KeyID::B, ::Agui::KeyID::B, true);
        KEY_TRAITS_CREATOR(KeyID::C, ::Agui::KeyID::C, true);
        KEY_TRAITS_CREATOR(KeyID::D, ::Agui::KeyID::D, true);
        KEY_TRAITS_CREATOR(KeyID::E, ::Agui::KeyID::E, true);
        KEY_TRAITS_CREATOR(KeyID::F, ::Agui::KeyID::F, true);
        KEY_TRAITS_CREATOR(KeyID::G, ::Agui::KeyID::G, true);
        KEY_TRAITS_CREATOR(KeyID::H, ::Agui::KeyID::H, true);
        KEY_TRAITS_CREATOR(KeyID::I, ::Agui::KeyID::I, true);
        KEY_TRAITS_CREATOR(KeyID::J, ::Agui::KeyID::J, true);
        KEY_TRAITS_CREATOR(KeyID::K, ::Agui::KeyID::K, true);
        KEY_TRAITS_CREATOR(KeyID::L, ::Agui::KeyID::L, true);
        KEY_TRAITS_CREATOR(KeyID::M, ::Agui::KeyID::M, true);
        KEY_TRAITS_CREATOR(KeyID::N, ::Agui::KeyID::N, true);
        KEY_TRAITS_CREATOR(KeyID::O, ::Agui::KeyID::O, true);
        KEY_TRAITS_CREATOR(KeyID::P, ::Agui::KeyID::P, true);
        KEY_TRAITS_CREATOR(KeyID::Q, ::Agui::KeyID::Q, true);
        KEY_TRAITS_CREATOR(KeyID::R, ::Agui::KeyID::R, true);
        KEY_TRAITS_CREATOR(KeyID::S, ::Agui::KeyID::S, true);
        KEY_TRAITS_CREATOR(KeyID::T, ::Agui::KeyID::T, true);
        KEY_TRAITS_CREATOR(KeyID::U, ::Agui::KeyID::U, true);
        KEY_TRAITS_CREATOR(KeyID::V, ::Agui::KeyID::V, true);
        KEY_TRAITS_CREATOR(KeyID::W, ::Agui::KeyID::W, true);
        KEY_TRAITS_CREATOR(KeyID::X, ::Agui::KeyID::X, true);
        KEY_TRAITS_CREATOR(KeyID::Y, ::Agui::KeyID::Y, true);
        KEY_TRAITS_CREATOR(KeyID::Z, ::Agui::KeyID::Z, true);

        KEY_TRAITS_CREATOR(KeyID::LEFT_ARROW, ::Agui::KeyID::LEFT_ARROW, true);
        KEY_TRAITS_CREATOR(KeyID::UP_ARROW, ::Agui::KeyID::UP_ARROW, true);
        KEY_TRAITS_CREATOR(KeyID::RIGHT_ARROW, ::Agui::KeyID::RIGHT_ARROW, true);
        KEY_TRAITS_CREATOR(KeyID::DOWN_ARROW, ::Agui::KeyID::DOWN_ARROW, true);

        KEY_TRAITS_CREATOR(KeyID::TAB, ::Agui::KeyID::TAB, true);
        KEY_TRAITS_CREATOR(KeyID::SPACEBAR, ::Agui::KeyID::SPACEBAR, true);
        KEY_TRAITS_CREATOR(KeyID::ENTER, ::Agui::KeyID::ENTER, true);
        KEY_TRAITS_CREATOR(KeyID::ESCAPE, ::Agui::KeyID::ESCAPE, true);
        KEY_TRAITS_CREATOR(KeyID::INSERT, ::Agui::KeyID::INSERT, true);
        KEY_TRAITS_CREATOR(KeyID::HOME, ::Agui::KeyID::HOME, true);
        KEY_TRAITS_CREATOR(KeyID::PAGE_UP, ::Agui::KeyID::PAGE_UP, true);
        KEY_TRAITS_CREATOR(KeyID::PAGE_DOWN, ::Agui::KeyID::PAGE_DOWN, true);
        KEY_TRAITS_CREATOR(KeyID::DEL, ::Agui::KeyID::DEL, true);
        KEY_TRAITS_CREATOR(KeyID::END, ::Agui::KeyID::END, true);

        KEY_TRAITS_CREATOR(KeyID::CANCEL, ::Agui::KeyID::CANCEL, true);
        KEY_TRAITS_CREATOR(KeyID::BACKSPACE, ::Agui::KeyID::BACKSPACE, true);
        KEY_TRAITS_CREATOR(KeyID::CLEAR, ::Agui::KeyID::CLEAR, true);
        KEY_TRAITS_CREATOR(KeyID::PAUSE, ::Agui::KeyID::PAUSE, true);
        KEY_TRAITS_CREATOR(KeyID::SELECT, ::Agui::KeyID::SELECT, true);
        KEY_TRAITS_CREATOR(KeyID::PRINT, ::Agui::KeyID::PRINT, true);
        KEY_TRAITS_CREATOR(KeyID::PRINT_SCREEN, ::Agui::KeyID::PRINT_SCREEN, true);
        KEY_TRAITS_CREATOR(KeyID::DECIMAL, ::Agui::KeyID::DECIMAL, true);

        KEY_TRAITS_CREATOR(KeyID::ADD, ::Agui::KeyID::ADD, true);
        KEY_TRAITS_CREATOR(KeyID::SUBTRACT, ::Agui::KeyID::SUBTRACT, true);
        KEY_TRAITS_CREATOR(KeyID::MULTIPLY, ::Agui::KeyID::MULTIPLY, true);
        KEY_TRAITS_CREATOR(KeyID::DIVIDE, ::Agui::KeyID::DIVIDE, true);

        KEY_TRAITS_CREATOR(KeyID::CAPS_LOCK, ::Agui::KeyID::CAPS_LOCK, true);
        KEY_TRAITS_CREATOR(KeyID::NUM_LOCK, ::Agui::KeyID::NUM_LOCK, true);
        KEY_TRAITS_CREATOR(KeyID::SCROLL_LOCK, ::Agui::KeyID::SCROLL_LOCK, true);

        KEY_TRAITS_CREATOR(KeyID::L_BRACKET, ::Agui::KeyID::L_BRACKET, true);
        KEY_TRAITS_CREATOR(KeyID::R_BRACKET, ::Agui::KeyID::R_BRACKET, true);

        KEY_TRAITS_CREATOR(KeyID::F1, ::Agui::KeyID::F1, false);
        KEY_TRAITS_CREATOR(KeyID::F2, ::Agui::KeyID::F2, false);
        KEY_TRAITS_CREATOR(KeyID::F3, ::Agui::KeyID::F3, false);
        KEY_TRAITS_CREATOR(KeyID::F4, ::Agui::KeyID::F4, false);
        KEY_TRAITS_CREATOR(KeyID::F5, ::Agui::KeyID::F5, false);
        KEY_TRAITS_CREATOR(KeyID::F6, ::Agui::KeyID::F6, false);
        KEY_TRAITS_CREATOR(KeyID::F7, ::Agui::KeyID::F7, false);
        KEY_TRAITS_CREATOR(KeyID::F8, ::Agui::KeyID::F8, false);
        KEY_TRAITS_CREATOR(KeyID::F9, ::Agui::KeyID::F9, false);
        KEY_TRAITS_CREATOR(KeyID::F10, ::Agui::KeyID::F10, false);
        KEY_TRAITS_CREATOR(KeyID::F11, ::Agui::KeyID::F11, false);
        KEY_TRAITS_CREATOR(KeyID::F12, ::Agui::KeyID::F12, false);
        KEY_TRAITS_CREATOR(KeyID::F13, ::Agui::KeyID::F13, false);
        KEY_TRAITS_CREATOR(KeyID::F14, ::Agui::KeyID::F14, false);
        KEY_TRAITS_CREATOR(KeyID::F15, ::Agui::KeyID::F15, false);
        KEY_TRAITS_CREATOR(KeyID::F16, ::Agui::KeyID::F16, false);
        KEY_TRAITS_CREATOR(KeyID::F17, ::Agui::KeyID::F17, false);
        KEY_TRAITS_CREATOR(KeyID::F18, ::Agui::KeyID::F18, false);
        KEY_TRAITS_CREATOR(KeyID::F19, ::Agui::KeyID::F19, false);
        KEY_TRAITS_CREATOR(KeyID::F20, ::Agui::KeyID::F20, false);
        KEY_TRAITS_CREATOR(KeyID::F21, ::Agui::KeyID::F21, false);
        KEY_TRAITS_CREATOR(KeyID::F22, ::Agui::KeyID::F22, false);
        KEY_TRAITS_CREATOR(KeyID::F23, ::Agui::KeyID::F23, false);
        KEY_TRAITS_CREATOR(KeyID::F24, ::Agui::KeyID::F24, false);

        KEY_TRAITS_CREATOR(KeyID::NUMPAD_0, ::Agui::KeyID::NUMPAD_0, false);
        KEY_TRAITS_CREATOR(KeyID::NUMPAD_1, ::Agui::KeyID::NUMPAD_1, false);
        KEY_TRAITS_CREATOR(KeyID::NUMPAD_2, ::Agui::KeyID::NUMPAD_2, false);
        KEY_TRAITS_CREATOR(KeyID::NUMPAD_3, ::Agui::KeyID::NUMPAD_3, false);
        KEY_TRAITS_CREATOR(KeyID::NUMPAD_4, ::Agui::KeyID::NUMPAD_4, false);
        KEY_TRAITS_CREATOR(KeyID::NUMPAD_5, ::Agui::KeyID::NUMPAD_5, false);
        KEY_TRAITS_CREATOR(KeyID::NUMPAD_6, ::Agui::KeyID::NUMPAD_6, false);
        KEY_TRAITS_CREATOR(KeyID::NUMPAD_7, ::Agui::KeyID::NUMPAD_7, false);
        KEY_TRAITS_CREATOR(KeyID::NUMPAD_8, ::Agui::KeyID::NUMPAD_8, false);
        KEY_TRAITS_CREATOR(KeyID::NUMPAD_9, ::Agui::KeyID::NUMPAD_9, false);

        KEY_TRAITS_CREATOR(KeyID::L_SHIFT, ::Agui::KeyID::L_SHIFT, false);
        KEY_TRAITS_CREATOR(KeyID::R_SHIFT, ::Agui::KeyID::R_SHIFT, false);
        KEY_TRAITS_CREATOR(KeyID::L_CONTROL, ::Agui::KeyID::L_CONTROL, false);
        KEY_TRAITS_CREATOR(KeyID::R_CONTROL, ::Agui::KeyID::R_CONTROL, false);
        KEY_TRAITS_CREATOR(KeyID::L_ALT, ::Agui::KeyID::L_ALT, false);
        KEY_TRAITS_CREATOR(KeyID::R_ALT, ::Agui::KeyID::R_ALT, false);
        KEY_TRAITS_CREATOR(KeyID::L_WIN, ::Agui::KeyID::L_WIN, false);
        KEY_TRAITS_CREATOR(KeyID::R_WIN, ::Agui::KeyID::R_WIN, false);

#undef KEY_TRAITS_CREATOR

        template<MouseKeyID id>
        struct MouseTraits;

#define MOUSE_TRAITS_CREATOR(idPass, aguiIDPass)                \
template<>                                                      \
struct MouseTraits<idPass>                                      \
{                                                               \
    static constexpr MouseKeyID id = idPass;                    \
    static constexpr ::Agui::MouseKeyID aguiID = aguiIDPass;    \
    static const Name displayName;                              \
}

        MOUSE_TRAITS_CREATOR(MouseKeyID::LEFT, ::Agui::MouseKeyID::LEFT);
        MOUSE_TRAITS_CREATOR(MouseKeyID::RIGHT, ::Agui::MouseKeyID::RIGHT);

#undef MOUSE_TRAITS_CREATOR

        template<ActionID id>
        struct ActionTraits;

#define ACTION_TRAITS_CREATOR(idPass)       \
template<>                                  \
struct ActionTraits<idPass>                 \
{                                           \
    static constexpr ActionID id = idPass;  \
    static const Name displayName;          \
}

        ACTION_TRAITS_CREATOR(ActionID::MOVE_LEFT);
        ACTION_TRAITS_CREATOR(ActionID::MOVE_UP);
        ACTION_TRAITS_CREATOR(ActionID::MOVE_RIGHT);
        ACTION_TRAITS_CREATOR(ActionID::MOVE_DOWN);
        ACTION_TRAITS_CREATOR(ActionID::USE);
        ACTION_TRAITS_CREATOR(ActionID::NAVIGATE_MENU_LEFT);
        ACTION_TRAITS_CREATOR(ActionID::NAVIGATE_MENU_UP);
        ACTION_TRAITS_CREATOR(ActionID::NAVIGATE_MENU_RIGHT);
        ACTION_TRAITS_CREATOR(ActionID::NAVIGATE_MENU_DOWN);
        ACTION_TRAITS_CREATOR(ActionID::CHANGE_SELECTED_CHARACTER_LEFT);
        ACTION_TRAITS_CREATOR(ActionID::CHANGE_SELECTED_CHARACTER_RIGHT);
        ACTION_TRAITS_CREATOR(ActionID::INVENTORY);
        ACTION_TRAITS_CREATOR(ActionID::END_TURN);
        ACTION_TRAITS_CREATOR(ActionID::ATTACK);
        ACTION_TRAITS_CREATOR(ActionID::OPEN_SPELLS);
        ACTION_TRAITS_CREATOR(ActionID::CANCEL);
        ACTION_TRAITS_CREATOR(ActionID::STATS);

#undef ACTION_TRAITS_CREATOR
    }
}