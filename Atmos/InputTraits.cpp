
#include "InputTraits.h"

namespace Atmos
{
    namespace Input
    {
#define KEY_TRAITS_DEFINE(idPass, displayNamePass)    \
const String KeyTraits<idPass>::displayName = displayNamePass

        KEY_TRAITS_DEFINE(KeyID::A, "A");
        KEY_TRAITS_DEFINE(KeyID::B, "B");
        KEY_TRAITS_DEFINE(KeyID::C, "C");
        KEY_TRAITS_DEFINE(KeyID::D, "D");
        KEY_TRAITS_DEFINE(KeyID::E, "E");
        KEY_TRAITS_DEFINE(KeyID::F, "F");
        KEY_TRAITS_DEFINE(KeyID::G, "G");
        KEY_TRAITS_DEFINE(KeyID::H, "H");
        KEY_TRAITS_DEFINE(KeyID::I, "I");
        KEY_TRAITS_DEFINE(KeyID::J, "J");
        KEY_TRAITS_DEFINE(KeyID::K, "K");
        KEY_TRAITS_DEFINE(KeyID::L, "L");
        KEY_TRAITS_DEFINE(KeyID::M, "M");
        KEY_TRAITS_DEFINE(KeyID::N, "N");
        KEY_TRAITS_DEFINE(KeyID::O, "O");
        KEY_TRAITS_DEFINE(KeyID::P, "P");
        KEY_TRAITS_DEFINE(KeyID::Q, "Q");
        KEY_TRAITS_DEFINE(KeyID::R, "R");
        KEY_TRAITS_DEFINE(KeyID::S, "S");
        KEY_TRAITS_DEFINE(KeyID::T, "T");
        KEY_TRAITS_DEFINE(KeyID::U, "U");
        KEY_TRAITS_DEFINE(KeyID::V, "V");
        KEY_TRAITS_DEFINE(KeyID::W, "W");
        KEY_TRAITS_DEFINE(KeyID::X, "X");
        KEY_TRAITS_DEFINE(KeyID::Y, "Y");
        KEY_TRAITS_DEFINE(KeyID::Z, "Z");

        KEY_TRAITS_DEFINE(KeyID::LEFT_ARROW, "Left Arrow");
        KEY_TRAITS_DEFINE(KeyID::UP_ARROW, "Up Arrow");
        KEY_TRAITS_DEFINE(KeyID::RIGHT_ARROW, "Right Arrow");
        KEY_TRAITS_DEFINE(KeyID::DOWN_ARROW, "Down Arrow");

        KEY_TRAITS_DEFINE(KeyID::TAB, "Tab");
        KEY_TRAITS_DEFINE(KeyID::SPACEBAR, "Spacebar");
        KEY_TRAITS_DEFINE(KeyID::ENTER, "Enter");
        KEY_TRAITS_DEFINE(KeyID::ESCAPE, "Escape");
        KEY_TRAITS_DEFINE(KeyID::INSERT, "Insert");
        KEY_TRAITS_DEFINE(KeyID::HOME, "Home");
        KEY_TRAITS_DEFINE(KeyID::PAGE_UP, "Page Up");
        KEY_TRAITS_DEFINE(KeyID::PAGE_DOWN, "Page Down");
        KEY_TRAITS_DEFINE(KeyID::DEL, "Delete");
        KEY_TRAITS_DEFINE(KeyID::END, "End");

        KEY_TRAITS_DEFINE(KeyID::CANCEL, "Cancel");
        KEY_TRAITS_DEFINE(KeyID::BACKSPACE, "Backspace");
        KEY_TRAITS_DEFINE(KeyID::CLEAR, "Clear");
        KEY_TRAITS_DEFINE(KeyID::PAUSE, "Pause");
        KEY_TRAITS_DEFINE(KeyID::SELECT, "Select");
        KEY_TRAITS_DEFINE(KeyID::PRINT, "Print");
        KEY_TRAITS_DEFINE(KeyID::PRINT_SCREEN, "Print Screen");
        KEY_TRAITS_DEFINE(KeyID::DECIMAL, ".");

        KEY_TRAITS_DEFINE(KeyID::ADD, "+");
        KEY_TRAITS_DEFINE(KeyID::SUBTRACT, "-");
        KEY_TRAITS_DEFINE(KeyID::MULTIPLY, "*");
        KEY_TRAITS_DEFINE(KeyID::DIVIDE, "/");

        KEY_TRAITS_DEFINE(KeyID::CAPS_LOCK, "Caps Lock");
        KEY_TRAITS_DEFINE(KeyID::NUM_LOCK, "Num Lock");
        KEY_TRAITS_DEFINE(KeyID::SCROLL_LOCK, "Scroll Lock");

        KEY_TRAITS_DEFINE(KeyID::L_BRACKET, "[");
        KEY_TRAITS_DEFINE(KeyID::R_BRACKET, "]");

        KEY_TRAITS_DEFINE(KeyID::F1, "F1");
        KEY_TRAITS_DEFINE(KeyID::F2, "F2");
        KEY_TRAITS_DEFINE(KeyID::F3, "F3");
        KEY_TRAITS_DEFINE(KeyID::F4, "F4");
        KEY_TRAITS_DEFINE(KeyID::F5, "F5");
        KEY_TRAITS_DEFINE(KeyID::F6, "F6");
        KEY_TRAITS_DEFINE(KeyID::F7, "F7");
        KEY_TRAITS_DEFINE(KeyID::F8, "F8");
        KEY_TRAITS_DEFINE(KeyID::F9, "F9");
        KEY_TRAITS_DEFINE(KeyID::F10, "F10");
        KEY_TRAITS_DEFINE(KeyID::F11, "F11");
        KEY_TRAITS_DEFINE(KeyID::F12, "F12");
        KEY_TRAITS_DEFINE(KeyID::F13, "F13");
        KEY_TRAITS_DEFINE(KeyID::F14, "F14");
        KEY_TRAITS_DEFINE(KeyID::F15, "F15");
        KEY_TRAITS_DEFINE(KeyID::F16, "F16");
        KEY_TRAITS_DEFINE(KeyID::F17, "F17");
        KEY_TRAITS_DEFINE(KeyID::F18, "F18");
        KEY_TRAITS_DEFINE(KeyID::F19, "F19");
        KEY_TRAITS_DEFINE(KeyID::F20, "F20");
        KEY_TRAITS_DEFINE(KeyID::F21, "F21");
        KEY_TRAITS_DEFINE(KeyID::F22, "F22");
        KEY_TRAITS_DEFINE(KeyID::F23, "F23");
        KEY_TRAITS_DEFINE(KeyID::F24, "F24");

        KEY_TRAITS_DEFINE(KeyID::NUMPAD_0, "Numpad 0");
        KEY_TRAITS_DEFINE(KeyID::NUMPAD_1, "Numpad 1");
        KEY_TRAITS_DEFINE(KeyID::NUMPAD_2, "Numpad 2");
        KEY_TRAITS_DEFINE(KeyID::NUMPAD_3, "Numpad 3");
        KEY_TRAITS_DEFINE(KeyID::NUMPAD_4, "Numpad 4");
        KEY_TRAITS_DEFINE(KeyID::NUMPAD_5, "Numpad 5");
        KEY_TRAITS_DEFINE(KeyID::NUMPAD_6, "Numpad 6");
        KEY_TRAITS_DEFINE(KeyID::NUMPAD_7, "Numpad 7");
        KEY_TRAITS_DEFINE(KeyID::NUMPAD_8, "Numpad 8");
        KEY_TRAITS_DEFINE(KeyID::NUMPAD_9, "Numpad 9");

        KEY_TRAITS_DEFINE(KeyID::L_SHIFT, "Left Shift");
        KEY_TRAITS_DEFINE(KeyID::R_SHIFT, "Right Shift");
        KEY_TRAITS_DEFINE(KeyID::L_CONTROL, "Left Control");
        KEY_TRAITS_DEFINE(KeyID::R_CONTROL, "Right Control");
        KEY_TRAITS_DEFINE(KeyID::L_ALT, "Left Alt");
        KEY_TRAITS_DEFINE(KeyID::R_ALT, "Right Alt");
        KEY_TRAITS_DEFINE(KeyID::L_WIN, "Left Windows");
        KEY_TRAITS_DEFINE(KeyID::R_WIN, "Right Windows");

#undef KEY_TRAITS_DEFINE

#define MOUSE_TRAITS_DEFINE(idPass, displayNamePass)    \
const String MouseTraits<idPass>::displayName = displayNamePass

        MOUSE_TRAITS_DEFINE(MouseKeyID::LEFT, "Left Mouse Button");
        MOUSE_TRAITS_DEFINE(MouseKeyID::RIGHT, "Right Mouse Button");

#undef MOUSE_TRAITS_DEFINE

#define ACTION_TRAITS_DEFINE(idPass, displayNamePass)               \
const String ActionTraits<idPass>::displayName = displayNamePass;   \

        ACTION_TRAITS_DEFINE(ActionID::MOVE_LEFT, "Move Left");
        ACTION_TRAITS_DEFINE(ActionID::MOVE_UP, "Move Up");
        ACTION_TRAITS_DEFINE(ActionID::MOVE_RIGHT, "Move Right");
        ACTION_TRAITS_DEFINE(ActionID::MOVE_DOWN, "Move Down");
        ACTION_TRAITS_DEFINE(ActionID::USE, "Use");
        ACTION_TRAITS_DEFINE(ActionID::NAVIGATE_MENU_LEFT, "Menu Left");
        ACTION_TRAITS_DEFINE(ActionID::NAVIGATE_MENU_UP, "Menu Up");
        ACTION_TRAITS_DEFINE(ActionID::NAVIGATE_MENU_RIGHT, "Menu Right");
        ACTION_TRAITS_DEFINE(ActionID::NAVIGATE_MENU_DOWN, "Menu Down");
        ACTION_TRAITS_DEFINE(ActionID::CHANGE_SELECTED_CHARACTER_LEFT, "Select Character Left");
        ACTION_TRAITS_DEFINE(ActionID::CHANGE_SELECTED_CHARACTER_RIGHT, "Select Character Right");
        ACTION_TRAITS_DEFINE(ActionID::INVENTORY, "Inventory");
        ACTION_TRAITS_DEFINE(ActionID::END_TURN, "End Turn");
        ACTION_TRAITS_DEFINE(ActionID::ATTACK, "Attack");
        ACTION_TRAITS_DEFINE(ActionID::OPEN_SPELLS, "Spells");
        ACTION_TRAITS_DEFINE(ActionID::CANCEL, "Cancel");
        ACTION_TRAITS_DEFINE(ActionID::STATS, "Stats");

#undef ACTION_TRAITS_DEFINE
    }
}