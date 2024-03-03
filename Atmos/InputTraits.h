
#pragma once

#include "InputKeyID.h"
#include "InputMouseKeyID.h"
#include "InputActionID.h"

#include "Name.h"

#include <Chroma/Enum.h>

namespace Atmos::Input
{
    typedef ::Chroma::EnumIterationTraits<KeyID, KeyID::A, KeyID::L_ALT> KeyIterationTraits;
    typedef ::Chroma::EnumIterationTraits<MouseKeyID, MouseKeyID::LEFT, MouseKeyID::RIGHT> MouseIterationTraits;
    typedef ::Chroma::EnumIterationTraits<ActionID, ActionID::MOVE_UP, ActionID::STATS> ActionIterationTraits;

    template<KeyID id>
    struct KeyTraits;

#define KEY_TRAITS_CREATOR(idPass, canUseForActionPass)             \
template<>                                                          \
struct KeyTraits<idPass>                                            \
{                                                                   \
    static constexpr KeyID id = idPass;                             \
    static const Name displayName;                                  \
    static constexpr bool canUseForAction = canUseForActionPass;    \
}

    KEY_TRAITS_CREATOR(KeyID::A, true);
    KEY_TRAITS_CREATOR(KeyID::B, true);
    KEY_TRAITS_CREATOR(KeyID::C, true);
    KEY_TRAITS_CREATOR(KeyID::D, true);
    KEY_TRAITS_CREATOR(KeyID::E, true);
    KEY_TRAITS_CREATOR(KeyID::F, true);
    KEY_TRAITS_CREATOR(KeyID::G, true);
    KEY_TRAITS_CREATOR(KeyID::H, true);
    KEY_TRAITS_CREATOR(KeyID::I, true);
    KEY_TRAITS_CREATOR(KeyID::J, true);
    KEY_TRAITS_CREATOR(KeyID::K, true);
    KEY_TRAITS_CREATOR(KeyID::L, true);
    KEY_TRAITS_CREATOR(KeyID::M, true);
    KEY_TRAITS_CREATOR(KeyID::N, true);
    KEY_TRAITS_CREATOR(KeyID::O, true);
    KEY_TRAITS_CREATOR(KeyID::P, true);
    KEY_TRAITS_CREATOR(KeyID::Q, true);
    KEY_TRAITS_CREATOR(KeyID::R, true);
    KEY_TRAITS_CREATOR(KeyID::S, true);
    KEY_TRAITS_CREATOR(KeyID::T, true);
    KEY_TRAITS_CREATOR(KeyID::U, true);
    KEY_TRAITS_CREATOR(KeyID::V, true);
    KEY_TRAITS_CREATOR(KeyID::W, true);
    KEY_TRAITS_CREATOR(KeyID::X, true);
    KEY_TRAITS_CREATOR(KeyID::Y, true);
    KEY_TRAITS_CREATOR(KeyID::Z, true);

    KEY_TRAITS_CREATOR(KeyID::LEFT_ARROW, true);
    KEY_TRAITS_CREATOR(KeyID::UP_ARROW, true);
    KEY_TRAITS_CREATOR(KeyID::RIGHT_ARROW, true);
    KEY_TRAITS_CREATOR(KeyID::DOWN_ARROW, true);

    KEY_TRAITS_CREATOR(KeyID::TAB, true);
    KEY_TRAITS_CREATOR(KeyID::SPACEBAR, true);
    KEY_TRAITS_CREATOR(KeyID::ENTER, true);
    KEY_TRAITS_CREATOR(KeyID::ESCAPE, true);
    KEY_TRAITS_CREATOR(KeyID::INSERT, true);
    KEY_TRAITS_CREATOR(KeyID::HOME, true);
    KEY_TRAITS_CREATOR(KeyID::PAGE_UP, true);
    KEY_TRAITS_CREATOR(KeyID::PAGE_DOWN, true);
    KEY_TRAITS_CREATOR(KeyID::DEL, true);
    KEY_TRAITS_CREATOR(KeyID::END, true);

    KEY_TRAITS_CREATOR(KeyID::CANCEL, true);
    KEY_TRAITS_CREATOR(KeyID::BACKSPACE, true);
    KEY_TRAITS_CREATOR(KeyID::CLEAR, true);
    KEY_TRAITS_CREATOR(KeyID::PAUSE, true);
    KEY_TRAITS_CREATOR(KeyID::SELECT, true);
    KEY_TRAITS_CREATOR(KeyID::PRINT, true);
    KEY_TRAITS_CREATOR(KeyID::PRINT_SCREEN, true);
    KEY_TRAITS_CREATOR(KeyID::DECIMAL, true);

    KEY_TRAITS_CREATOR(KeyID::ADD, true);
    KEY_TRAITS_CREATOR(KeyID::SUBTRACT, true);
    KEY_TRAITS_CREATOR(KeyID::MULTIPLY,  true);
    KEY_TRAITS_CREATOR(KeyID::DIVIDE, true);

    KEY_TRAITS_CREATOR(KeyID::CAPS_LOCK, true);
    KEY_TRAITS_CREATOR(KeyID::NUM_LOCK, true);
    KEY_TRAITS_CREATOR(KeyID::SCROLL_LOCK, true);

    KEY_TRAITS_CREATOR(KeyID::L_BRACKET, true);
    KEY_TRAITS_CREATOR(KeyID::R_BRACKET, true);

    KEY_TRAITS_CREATOR(KeyID::F1, false);
    KEY_TRAITS_CREATOR(KeyID::F2, false);
    KEY_TRAITS_CREATOR(KeyID::F3, false);
    KEY_TRAITS_CREATOR(KeyID::F4, false);
    KEY_TRAITS_CREATOR(KeyID::F5, false);
    KEY_TRAITS_CREATOR(KeyID::F6, false);
    KEY_TRAITS_CREATOR(KeyID::F7, false);
    KEY_TRAITS_CREATOR(KeyID::F8, false);
    KEY_TRAITS_CREATOR(KeyID::F9, false);
    KEY_TRAITS_CREATOR(KeyID::F10, false);
    KEY_TRAITS_CREATOR(KeyID::F11, false);
    KEY_TRAITS_CREATOR(KeyID::F12, false);
    KEY_TRAITS_CREATOR(KeyID::F13, false);
    KEY_TRAITS_CREATOR(KeyID::F14, false);
    KEY_TRAITS_CREATOR(KeyID::F15, false);
    KEY_TRAITS_CREATOR(KeyID::F16, false);
    KEY_TRAITS_CREATOR(KeyID::F17, false);
    KEY_TRAITS_CREATOR(KeyID::F18, false);
    KEY_TRAITS_CREATOR(KeyID::F19, false);
    KEY_TRAITS_CREATOR(KeyID::F20, false);
    KEY_TRAITS_CREATOR(KeyID::F21, false);
    KEY_TRAITS_CREATOR(KeyID::F22, false);
    KEY_TRAITS_CREATOR(KeyID::F23, false);
    KEY_TRAITS_CREATOR(KeyID::F24, false);

    KEY_TRAITS_CREATOR(KeyID::NUMPAD_0, false);
    KEY_TRAITS_CREATOR(KeyID::NUMPAD_1, false);
    KEY_TRAITS_CREATOR(KeyID::NUMPAD_2, false);
    KEY_TRAITS_CREATOR(KeyID::NUMPAD_3, false);
    KEY_TRAITS_CREATOR(KeyID::NUMPAD_4, false);
    KEY_TRAITS_CREATOR(KeyID::NUMPAD_5, false);
    KEY_TRAITS_CREATOR(KeyID::NUMPAD_6, false);
    KEY_TRAITS_CREATOR(KeyID::NUMPAD_7, false);
    KEY_TRAITS_CREATOR(KeyID::NUMPAD_8, false);
    KEY_TRAITS_CREATOR(KeyID::NUMPAD_9, false);

    KEY_TRAITS_CREATOR(KeyID::L_SHIFT, false);
    KEY_TRAITS_CREATOR(KeyID::R_SHIFT, false);
    KEY_TRAITS_CREATOR(KeyID::L_CONTROL, false);
    KEY_TRAITS_CREATOR(KeyID::R_CONTROL, false);
    KEY_TRAITS_CREATOR(KeyID::L_ALT, false);
    KEY_TRAITS_CREATOR(KeyID::R_ALT, false);
    KEY_TRAITS_CREATOR(KeyID::L_WIN, false);
    KEY_TRAITS_CREATOR(KeyID::R_WIN, false);

#undef KEY_TRAITS_CREATOR

    template<MouseKeyID id>
    struct MouseTraits;

#define MOUSE_TRAITS_CREATOR(idPass)        \
template<>                                  \
struct MouseTraits<idPass>                  \
{                                           \
    static constexpr MouseKeyID id = idPass;\
    static const Name displayName;          \
}

    MOUSE_TRAITS_CREATOR(MouseKeyID::LEFT);
    MOUSE_TRAITS_CREATOR(MouseKeyID::RIGHT);

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