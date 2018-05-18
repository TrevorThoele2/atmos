
#pragma once

#include <AGUI\Input.h>
#include <Function\Enum.h>
#include "Name.h"
#include "IniID.h"

namespace Atmos
{
    namespace Input
    {
        enum class KeyID
        {
            NONE,

            A,
            B,
            C,
            D,
            E,
            F,
            G,
            H,
            I,
            J,
            K,
            L,
            M,
            N,
            O,
            P,
            Q,
            R,
            S,
            T,
            U,
            V,
            W,
            X,
            Y,
            Z,

            UP_ARROW,
            DOWN_ARROW,
            LEFT_ARROW,
            RIGHT_ARROW,

            TAB,
            SPACEBAR,
            ENTER,
            ESCAPE,
            INSERT,
            HOME,
            PAGE_UP,
            PAGE_DOWN,
            DEL,
            END,

            CANCEL,
            BACKSPACE,
            CLEAR,
            PAUSE,
            SELECT,
            PRINT,
            PRINT_SCREEN,
            DECIMAL,

            ADD,
            SUBTRACT,
            MULTIPLY,
            DIVIDE,

            CAPS_LOCK,
            NUM_LOCK,
            SCROLL_LOCK,

            L_BRACKET,
            R_BRACKET,

            F1,
            F2,
            F3,
            F4,
            F5,
            F6,
            F7,
            F8,
            F9,
            F10,
            F11,
            F12,
            F13,
            F14,
            F15,
            F16,
            F17,
            F18,
            F19,
            F20,
            F21,
            F22,
            F23,
            F24,

            NUMPAD_0,
            NUMPAD_1,
            NUMPAD_2,
            NUMPAD_3,
            NUMPAD_4,
            NUMPAD_5,
            NUMPAD_6,
            NUMPAD_7,
            NUMPAD_8,
            NUMPAD_9,

            L_SHIFT,
            R_SHIFT,
            L_CONTROL,
            R_CONTROL,
            L_ALT,
            R_ALT,
            L_WIN,
            R_WIN
        };

        enum class MouseButton
        {
            NONE,
            LEFT,
            RIGHT
        };

        enum class ActionID
        {
            NONE,
            MOVE_UP,
            MOVE_DOWN,
            MOVE_LEFT,
            MOVE_RIGHT,
            USE,
            NAVIGATE_MENU_UP,
            NAVIGATE_MENU_DOWN,
            NAVIGATE_MENU_LEFT,
            NAVIGATE_MENU_RIGHT,
            CHANGE_SELECTED_CHARACTER_LEFT,
            CHANGE_SELECTED_CHARACTER_RIGHT,
            INVENTORY,
            END_TURN,
            ATTACK,
            OPEN_SPELLS,
            CANCEL,
            STATS
        };

        typedef function::EnumIterationTraits<KeyID, KeyID::A, KeyID::L_ALT> KeyIterationTraits;
        typedef function::EnumIterationTraits<MouseButton, MouseButton::LEFT, MouseButton::RIGHT> MouseIterationTraits;
        typedef function::EnumIterationTraits<ActionID, ActionID::MOVE_UP, ActionID::STATS> ActionIterationTraits;

        template<KeyID id>
        struct KeyTraits;

#define KEY_TRAITS_CREATOR(idPass, aguiIDPass, canUseForActionPass) \
template<>                                                          \
struct KeyTraits<idPass>                                            \
{                                                                   \
    static constexpr KeyID id = idPass;                             \
    static constexpr agui::Key aguiID = aguiIDPass;                 \
    static const Name displayName;                                  \
    static constexpr bool canUseForAction = canUseForActionPass;    \
}

        KEY_TRAITS_CREATOR(KeyID::A, agui::Key::A, true);
        KEY_TRAITS_CREATOR(KeyID::B, agui::Key::B, true);
        KEY_TRAITS_CREATOR(KeyID::C, agui::Key::C, true);
        KEY_TRAITS_CREATOR(KeyID::D, agui::Key::D, true);
        KEY_TRAITS_CREATOR(KeyID::E, agui::Key::E, true);
        KEY_TRAITS_CREATOR(KeyID::F, agui::Key::F, true);
        KEY_TRAITS_CREATOR(KeyID::G, agui::Key::G, true);
        KEY_TRAITS_CREATOR(KeyID::H, agui::Key::H, true);
        KEY_TRAITS_CREATOR(KeyID::I, agui::Key::I, true);
        KEY_TRAITS_CREATOR(KeyID::J, agui::Key::J, true);
        KEY_TRAITS_CREATOR(KeyID::K, agui::Key::K, true);
        KEY_TRAITS_CREATOR(KeyID::L, agui::Key::L, true);
        KEY_TRAITS_CREATOR(KeyID::M, agui::Key::M, true);
        KEY_TRAITS_CREATOR(KeyID::N, agui::Key::N, true);
        KEY_TRAITS_CREATOR(KeyID::O, agui::Key::O, true);
        KEY_TRAITS_CREATOR(KeyID::P, agui::Key::P, true);
        KEY_TRAITS_CREATOR(KeyID::Q, agui::Key::Q, true);
        KEY_TRAITS_CREATOR(KeyID::R, agui::Key::R, true);
        KEY_TRAITS_CREATOR(KeyID::S, agui::Key::S, true);
        KEY_TRAITS_CREATOR(KeyID::T, agui::Key::T, true);
        KEY_TRAITS_CREATOR(KeyID::U, agui::Key::U, true);
        KEY_TRAITS_CREATOR(KeyID::V, agui::Key::V, true);
        KEY_TRAITS_CREATOR(KeyID::W, agui::Key::W, true);
        KEY_TRAITS_CREATOR(KeyID::X, agui::Key::X, true);
        KEY_TRAITS_CREATOR(KeyID::Y, agui::Key::Y, true);
        KEY_TRAITS_CREATOR(KeyID::Z, agui::Key::Z, true);

        KEY_TRAITS_CREATOR(KeyID::LEFT_ARROW, agui::Key::LEFT_ARROW, true);
        KEY_TRAITS_CREATOR(KeyID::UP_ARROW, agui::Key::UP_ARROW, true);
        KEY_TRAITS_CREATOR(KeyID::RIGHT_ARROW, agui::Key::RIGHT_ARROW, true);
        KEY_TRAITS_CREATOR(KeyID::DOWN_ARROW, agui::Key::DOWN_ARROW, true);

        KEY_TRAITS_CREATOR(KeyID::TAB, agui::Key::TAB, true);
        KEY_TRAITS_CREATOR(KeyID::SPACEBAR, agui::Key::SPACEBAR, true);
        KEY_TRAITS_CREATOR(KeyID::ENTER, agui::Key::ENTER, true);
        KEY_TRAITS_CREATOR(KeyID::ESCAPE, agui::Key::ESCAPE, true);
        KEY_TRAITS_CREATOR(KeyID::INSERT, agui::Key::INSERT, true);
        KEY_TRAITS_CREATOR(KeyID::HOME, agui::Key::HOME, true);
        KEY_TRAITS_CREATOR(KeyID::PAGE_UP, agui::Key::PAGE_UP, true);
        KEY_TRAITS_CREATOR(KeyID::PAGE_DOWN, agui::Key::PAGE_DOWN, true);
        KEY_TRAITS_CREATOR(KeyID::DEL, agui::Key::DEL, true);
        KEY_TRAITS_CREATOR(KeyID::END, agui::Key::END, true);

        KEY_TRAITS_CREATOR(KeyID::CANCEL, agui::Key::CANCEL, true);
        KEY_TRAITS_CREATOR(KeyID::BACKSPACE, agui::Key::BACKSPACE, true);
        KEY_TRAITS_CREATOR(KeyID::CLEAR, agui::Key::CLEAR, true);
        KEY_TRAITS_CREATOR(KeyID::PAUSE, agui::Key::PAUSE, true);
        KEY_TRAITS_CREATOR(KeyID::SELECT, agui::Key::SELECT, true);
        KEY_TRAITS_CREATOR(KeyID::PRINT, agui::Key::PRINT, true);
        KEY_TRAITS_CREATOR(KeyID::PRINT_SCREEN, agui::Key::PRINT_SCREEN, true);
        KEY_TRAITS_CREATOR(KeyID::DECIMAL, agui::Key::DECIMAL, true);

        KEY_TRAITS_CREATOR(KeyID::ADD, agui::Key::ADD, true);
        KEY_TRAITS_CREATOR(KeyID::SUBTRACT, agui::Key::SUBTRACT, true);
        KEY_TRAITS_CREATOR(KeyID::MULTIPLY, agui::Key::MULTIPLY, true);
        KEY_TRAITS_CREATOR(KeyID::DIVIDE, agui::Key::DIVIDE, true);

        KEY_TRAITS_CREATOR(KeyID::CAPS_LOCK, agui::Key::CAPS_LOCK, true);
        KEY_TRAITS_CREATOR(KeyID::NUM_LOCK, agui::Key::NUM_LOCK, true);
        KEY_TRAITS_CREATOR(KeyID::SCROLL_LOCK, agui::Key::SCROLL_LOCK, true);

        KEY_TRAITS_CREATOR(KeyID::L_BRACKET, agui::Key::L_BRACKET, true);
        KEY_TRAITS_CREATOR(KeyID::R_BRACKET, agui::Key::R_BRACKET, true);

        KEY_TRAITS_CREATOR(KeyID::F1, agui::Key::F1, false);
        KEY_TRAITS_CREATOR(KeyID::F2, agui::Key::F2, false);
        KEY_TRAITS_CREATOR(KeyID::F3, agui::Key::F3, false);
        KEY_TRAITS_CREATOR(KeyID::F4, agui::Key::F4, false);
        KEY_TRAITS_CREATOR(KeyID::F5, agui::Key::F5, false);
        KEY_TRAITS_CREATOR(KeyID::F6, agui::Key::F6, false);
        KEY_TRAITS_CREATOR(KeyID::F7, agui::Key::F7, false);
        KEY_TRAITS_CREATOR(KeyID::F8, agui::Key::F8, false);
        KEY_TRAITS_CREATOR(KeyID::F9, agui::Key::F9, false);
        KEY_TRAITS_CREATOR(KeyID::F10, agui::Key::F10, false);
        KEY_TRAITS_CREATOR(KeyID::F11, agui::Key::F11, false);
        KEY_TRAITS_CREATOR(KeyID::F12, agui::Key::F12, false);
        KEY_TRAITS_CREATOR(KeyID::F13, agui::Key::F13, false);
        KEY_TRAITS_CREATOR(KeyID::F14, agui::Key::F14, false);
        KEY_TRAITS_CREATOR(KeyID::F15, agui::Key::F15, false);
        KEY_TRAITS_CREATOR(KeyID::F16, agui::Key::F16, false);
        KEY_TRAITS_CREATOR(KeyID::F17, agui::Key::F17, false);
        KEY_TRAITS_CREATOR(KeyID::F18, agui::Key::F18, false);
        KEY_TRAITS_CREATOR(KeyID::F19, agui::Key::F19, false);
        KEY_TRAITS_CREATOR(KeyID::F20, agui::Key::F20, false);
        KEY_TRAITS_CREATOR(KeyID::F21, agui::Key::F21, false);
        KEY_TRAITS_CREATOR(KeyID::F22, agui::Key::F22, false);
        KEY_TRAITS_CREATOR(KeyID::F23, agui::Key::F23, false);
        KEY_TRAITS_CREATOR(KeyID::F24, agui::Key::F24, false);

        KEY_TRAITS_CREATOR(KeyID::NUMPAD_0, agui::Key::NUMPAD_0, false);
        KEY_TRAITS_CREATOR(KeyID::NUMPAD_1, agui::Key::NUMPAD_1, false);
        KEY_TRAITS_CREATOR(KeyID::NUMPAD_2, agui::Key::NUMPAD_2, false);
        KEY_TRAITS_CREATOR(KeyID::NUMPAD_3, agui::Key::NUMPAD_3, false);
        KEY_TRAITS_CREATOR(KeyID::NUMPAD_4, agui::Key::NUMPAD_4, false);
        KEY_TRAITS_CREATOR(KeyID::NUMPAD_5, agui::Key::NUMPAD_5, false);
        KEY_TRAITS_CREATOR(KeyID::NUMPAD_6, agui::Key::NUMPAD_6, false);
        KEY_TRAITS_CREATOR(KeyID::NUMPAD_7, agui::Key::NUMPAD_7, false);
        KEY_TRAITS_CREATOR(KeyID::NUMPAD_8, agui::Key::NUMPAD_8, false);
        KEY_TRAITS_CREATOR(KeyID::NUMPAD_9, agui::Key::NUMPAD_9, false);

        KEY_TRAITS_CREATOR(KeyID::L_SHIFT, agui::Key::L_SHIFT, false);
        KEY_TRAITS_CREATOR(KeyID::R_SHIFT, agui::Key::R_SHIFT, false);
        KEY_TRAITS_CREATOR(KeyID::L_CONTROL, agui::Key::L_CONTROL, false);
        KEY_TRAITS_CREATOR(KeyID::R_CONTROL, agui::Key::R_CONTROL, false);
        KEY_TRAITS_CREATOR(KeyID::L_ALT, agui::Key::L_ALT, false);
        KEY_TRAITS_CREATOR(KeyID::R_ALT, agui::Key::R_ALT, false);
        KEY_TRAITS_CREATOR(KeyID::L_WIN, agui::Key::L_WIN, false);
        KEY_TRAITS_CREATOR(KeyID::R_WIN, agui::Key::R_WIN, false);

#undef KEY_TRAITS_CREATOR

        template<MouseButton id>
        struct MouseTraits;

#define MOUSE_TRAITS_CREATOR(idPass, aguiIDPass)            \
template<>                                                  \
struct MouseTraits<idPass>                                  \
{                                                           \
    static constexpr MouseButton id = idPass;               \
    static constexpr agui::MouseButton aguiID = aguiIDPass; \
    static const Name displayName;                          \
}

        MOUSE_TRAITS_CREATOR(MouseButton::LEFT, agui::MouseButton::LEFT);
        MOUSE_TRAITS_CREATOR(MouseButton::RIGHT, agui::MouseButton::RIGHT);

#undef MOUSE_TRAITS_CREATOR

        template<ActionID id>
        struct ActionTraits;

#define ACTION_TRAITS_CREATOR(idPass)       \
template<>                                  \
struct ActionTraits<idPass>                 \
{                                           \
    static constexpr ActionID id = idPass;  \
    static const Name displayName;          \
    static const Ini::ID iniID;             \
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