
#pragma once

#include <AGUI\Input.h>
#include <Chroma\Enum.h>
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

        typedef ::Chroma::EnumIterationTraits<KeyID, KeyID::A, KeyID::L_ALT> KeyIterationTraits;
        typedef ::Chroma::EnumIterationTraits<MouseButton, MouseButton::LEFT, MouseButton::RIGHT> MouseIterationTraits;
        typedef ::Chroma::EnumIterationTraits<ActionID, ActionID::MOVE_UP, ActionID::STATS> ActionIterationTraits;

        template<KeyID id>
        struct KeyTraits;

#define KEY_TRAITS_CREATOR(idPass, aguiIDPass, canUseForActionPass) \
template<>                                                          \
struct KeyTraits<idPass>                                            \
{                                                                   \
    static constexpr KeyID id = idPass;                             \
    static constexpr Agui::Key aguiID = aguiIDPass;                 \
    static const Name displayName;                                  \
    static constexpr bool canUseForAction = canUseForActionPass;    \
}

        KEY_TRAITS_CREATOR(KeyID::A, Agui::Key::A, true);
        KEY_TRAITS_CREATOR(KeyID::B, Agui::Key::B, true);
        KEY_TRAITS_CREATOR(KeyID::C, Agui::Key::C, true);
        KEY_TRAITS_CREATOR(KeyID::D, Agui::Key::D, true);
        KEY_TRAITS_CREATOR(KeyID::E, Agui::Key::E, true);
        KEY_TRAITS_CREATOR(KeyID::F, Agui::Key::F, true);
        KEY_TRAITS_CREATOR(KeyID::G, Agui::Key::G, true);
        KEY_TRAITS_CREATOR(KeyID::H, Agui::Key::H, true);
        KEY_TRAITS_CREATOR(KeyID::I, Agui::Key::I, true);
        KEY_TRAITS_CREATOR(KeyID::J, Agui::Key::J, true);
        KEY_TRAITS_CREATOR(KeyID::K, Agui::Key::K, true);
        KEY_TRAITS_CREATOR(KeyID::L, Agui::Key::L, true);
        KEY_TRAITS_CREATOR(KeyID::M, Agui::Key::M, true);
        KEY_TRAITS_CREATOR(KeyID::N, Agui::Key::N, true);
        KEY_TRAITS_CREATOR(KeyID::O, Agui::Key::O, true);
        KEY_TRAITS_CREATOR(KeyID::P, Agui::Key::P, true);
        KEY_TRAITS_CREATOR(KeyID::Q, Agui::Key::Q, true);
        KEY_TRAITS_CREATOR(KeyID::R, Agui::Key::R, true);
        KEY_TRAITS_CREATOR(KeyID::S, Agui::Key::S, true);
        KEY_TRAITS_CREATOR(KeyID::T, Agui::Key::T, true);
        KEY_TRAITS_CREATOR(KeyID::U, Agui::Key::U, true);
        KEY_TRAITS_CREATOR(KeyID::V, Agui::Key::V, true);
        KEY_TRAITS_CREATOR(KeyID::W, Agui::Key::W, true);
        KEY_TRAITS_CREATOR(KeyID::X, Agui::Key::X, true);
        KEY_TRAITS_CREATOR(KeyID::Y, Agui::Key::Y, true);
        KEY_TRAITS_CREATOR(KeyID::Z, Agui::Key::Z, true);

        KEY_TRAITS_CREATOR(KeyID::LEFT_ARROW, Agui::Key::LEFT_ARROW, true);
        KEY_TRAITS_CREATOR(KeyID::UP_ARROW, Agui::Key::UP_ARROW, true);
        KEY_TRAITS_CREATOR(KeyID::RIGHT_ARROW, Agui::Key::RIGHT_ARROW, true);
        KEY_TRAITS_CREATOR(KeyID::DOWN_ARROW, Agui::Key::DOWN_ARROW, true);

        KEY_TRAITS_CREATOR(KeyID::TAB, Agui::Key::TAB, true);
        KEY_TRAITS_CREATOR(KeyID::SPACEBAR, Agui::Key::SPACEBAR, true);
        KEY_TRAITS_CREATOR(KeyID::ENTER, Agui::Key::ENTER, true);
        KEY_TRAITS_CREATOR(KeyID::ESCAPE, Agui::Key::ESCAPE, true);
        KEY_TRAITS_CREATOR(KeyID::INSERT, Agui::Key::INSERT, true);
        KEY_TRAITS_CREATOR(KeyID::HOME, Agui::Key::HOME, true);
        KEY_TRAITS_CREATOR(KeyID::PAGE_UP, Agui::Key::PAGE_UP, true);
        KEY_TRAITS_CREATOR(KeyID::PAGE_DOWN, Agui::Key::PAGE_DOWN, true);
        KEY_TRAITS_CREATOR(KeyID::DEL, Agui::Key::DEL, true);
        KEY_TRAITS_CREATOR(KeyID::END, Agui::Key::END, true);

        KEY_TRAITS_CREATOR(KeyID::CANCEL, Agui::Key::CANCEL, true);
        KEY_TRAITS_CREATOR(KeyID::BACKSPACE, Agui::Key::BACKSPACE, true);
        KEY_TRAITS_CREATOR(KeyID::CLEAR, Agui::Key::CLEAR, true);
        KEY_TRAITS_CREATOR(KeyID::PAUSE, Agui::Key::PAUSE, true);
        KEY_TRAITS_CREATOR(KeyID::SELECT, Agui::Key::SELECT, true);
        KEY_TRAITS_CREATOR(KeyID::PRINT, Agui::Key::PRINT, true);
        KEY_TRAITS_CREATOR(KeyID::PRINT_SCREEN, Agui::Key::PRINT_SCREEN, true);
        KEY_TRAITS_CREATOR(KeyID::DECIMAL, Agui::Key::DECIMAL, true);

        KEY_TRAITS_CREATOR(KeyID::ADD, Agui::Key::ADD, true);
        KEY_TRAITS_CREATOR(KeyID::SUBTRACT, Agui::Key::SUBTRACT, true);
        KEY_TRAITS_CREATOR(KeyID::MULTIPLY, Agui::Key::MULTIPLY, true);
        KEY_TRAITS_CREATOR(KeyID::DIVIDE, Agui::Key::DIVIDE, true);

        KEY_TRAITS_CREATOR(KeyID::CAPS_LOCK, Agui::Key::CAPS_LOCK, true);
        KEY_TRAITS_CREATOR(KeyID::NUM_LOCK, Agui::Key::NUM_LOCK, true);
        KEY_TRAITS_CREATOR(KeyID::SCROLL_LOCK, Agui::Key::SCROLL_LOCK, true);

        KEY_TRAITS_CREATOR(KeyID::L_BRACKET, Agui::Key::L_BRACKET, true);
        KEY_TRAITS_CREATOR(KeyID::R_BRACKET, Agui::Key::R_BRACKET, true);

        KEY_TRAITS_CREATOR(KeyID::F1, Agui::Key::F1, false);
        KEY_TRAITS_CREATOR(KeyID::F2, Agui::Key::F2, false);
        KEY_TRAITS_CREATOR(KeyID::F3, Agui::Key::F3, false);
        KEY_TRAITS_CREATOR(KeyID::F4, Agui::Key::F4, false);
        KEY_TRAITS_CREATOR(KeyID::F5, Agui::Key::F5, false);
        KEY_TRAITS_CREATOR(KeyID::F6, Agui::Key::F6, false);
        KEY_TRAITS_CREATOR(KeyID::F7, Agui::Key::F7, false);
        KEY_TRAITS_CREATOR(KeyID::F8, Agui::Key::F8, false);
        KEY_TRAITS_CREATOR(KeyID::F9, Agui::Key::F9, false);
        KEY_TRAITS_CREATOR(KeyID::F10, Agui::Key::F10, false);
        KEY_TRAITS_CREATOR(KeyID::F11, Agui::Key::F11, false);
        KEY_TRAITS_CREATOR(KeyID::F12, Agui::Key::F12, false);
        KEY_TRAITS_CREATOR(KeyID::F13, Agui::Key::F13, false);
        KEY_TRAITS_CREATOR(KeyID::F14, Agui::Key::F14, false);
        KEY_TRAITS_CREATOR(KeyID::F15, Agui::Key::F15, false);
        KEY_TRAITS_CREATOR(KeyID::F16, Agui::Key::F16, false);
        KEY_TRAITS_CREATOR(KeyID::F17, Agui::Key::F17, false);
        KEY_TRAITS_CREATOR(KeyID::F18, Agui::Key::F18, false);
        KEY_TRAITS_CREATOR(KeyID::F19, Agui::Key::F19, false);
        KEY_TRAITS_CREATOR(KeyID::F20, Agui::Key::F20, false);
        KEY_TRAITS_CREATOR(KeyID::F21, Agui::Key::F21, false);
        KEY_TRAITS_CREATOR(KeyID::F22, Agui::Key::F22, false);
        KEY_TRAITS_CREATOR(KeyID::F23, Agui::Key::F23, false);
        KEY_TRAITS_CREATOR(KeyID::F24, Agui::Key::F24, false);

        KEY_TRAITS_CREATOR(KeyID::NUMPAD_0, Agui::Key::NUMPAD_0, false);
        KEY_TRAITS_CREATOR(KeyID::NUMPAD_1, Agui::Key::NUMPAD_1, false);
        KEY_TRAITS_CREATOR(KeyID::NUMPAD_2, Agui::Key::NUMPAD_2, false);
        KEY_TRAITS_CREATOR(KeyID::NUMPAD_3, Agui::Key::NUMPAD_3, false);
        KEY_TRAITS_CREATOR(KeyID::NUMPAD_4, Agui::Key::NUMPAD_4, false);
        KEY_TRAITS_CREATOR(KeyID::NUMPAD_5, Agui::Key::NUMPAD_5, false);
        KEY_TRAITS_CREATOR(KeyID::NUMPAD_6, Agui::Key::NUMPAD_6, false);
        KEY_TRAITS_CREATOR(KeyID::NUMPAD_7, Agui::Key::NUMPAD_7, false);
        KEY_TRAITS_CREATOR(KeyID::NUMPAD_8, Agui::Key::NUMPAD_8, false);
        KEY_TRAITS_CREATOR(KeyID::NUMPAD_9, Agui::Key::NUMPAD_9, false);

        KEY_TRAITS_CREATOR(KeyID::L_SHIFT, Agui::Key::L_SHIFT, false);
        KEY_TRAITS_CREATOR(KeyID::R_SHIFT, Agui::Key::R_SHIFT, false);
        KEY_TRAITS_CREATOR(KeyID::L_CONTROL, Agui::Key::L_CONTROL, false);
        KEY_TRAITS_CREATOR(KeyID::R_CONTROL, Agui::Key::R_CONTROL, false);
        KEY_TRAITS_CREATOR(KeyID::L_ALT, Agui::Key::L_ALT, false);
        KEY_TRAITS_CREATOR(KeyID::R_ALT, Agui::Key::R_ALT, false);
        KEY_TRAITS_CREATOR(KeyID::L_WIN, Agui::Key::L_WIN, false);
        KEY_TRAITS_CREATOR(KeyID::R_WIN, Agui::Key::R_WIN, false);

#undef KEY_TRAITS_CREATOR

        template<MouseButton id>
        struct MouseTraits;

#define MOUSE_TRAITS_CREATOR(idPass, aguiIDPass)            \
template<>                                                  \
struct MouseTraits<idPass>                                  \
{                                                           \
    static constexpr MouseButton id = idPass;               \
    static constexpr Agui::MouseButton aguiID = aguiIDPass; \
    static const Name displayName;                          \
}

        MOUSE_TRAITS_CREATOR(MouseButton::LEFT, Agui::MouseButton::LEFT);
        MOUSE_TRAITS_CREATOR(MouseButton::RIGHT, Agui::MouseButton::RIGHT);

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