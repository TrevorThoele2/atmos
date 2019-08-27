#include "WindowsInputManager.h"
#include <utility>

namespace Atmos::Input
{
    class InputDataImplementation : public InputData
    {
    public:
        using Code = int;
        const Code code;

        explicit InputDataImplementation(Code code) : code(code)
        {}

        [[nodiscard]] std::unique_ptr<InputData> Clone() const override
        {
            return std::make_unique<InputDataImplementation>(code);
        }
    };

    WindowsManager::WindowsManager(HWND hwnd) : hwnd(hwnd)
    {}

    std::vector<WindowsManager::CreatedInput> WindowsManager::CreateAllInputs() const
    {
        std::vector<CreatedInput> createdInputs;
        const auto create = [&createdInputs](InputID id, String displayName, InputDataImplementation::Code code)
        {
            createdInputs.push_back({
                std::move(id),
                Input
                {
                    std::move(displayName),
                    std::make_unique<InputDataImplementation>(code)
                } });
        };

        create("leftmouse", "Left Mouse Button", VK_LBUTTON);
        create("rightmouse", "Right Mouse Button", VK_RBUTTON);
        create("a", "A", 0x41);
        create("b", "B", 0x42);
        create("c", "C", 0x43);
        create("d", "D", 0x44);
        create("e", "E", 0x45);
        create("f", "F", 0x46);
        create("g", "G", 0x47);
        create("h", "H", 0x48);
        create("i", "I", 0x49);
        create("j", "J", 0x4A);
        create("k", "K", 0x4B);
        create("l", "L", 0x4C);
        create("m", "M", 0x4D);
        create("n", "N", 0x4E);
        create("o", "O", 0x4F);
        create("p", "P", 0x50);
        create("q", "Q", 0x51);
        create("r", "R", 0x52);
        create("s", "S", 0x53);
        create("t", "T", 0x54);
        create("u", "U", 0x55);
        create("v", "V", 0x56);
        create("w", "W", 0x57);
        create("x", "X", 0x58);
        create("y", "Y", 0x59);
        create("z", "Z", 0x5A);

        create("left", "Left", VK_LEFT);
        create("up", "Up", VK_UP);
        create("right", "Right", VK_RIGHT);
        create("down", "Down", VK_DOWN);

        create("tab", "Tab", VK_TAB);
        create("spacebar", "Spacebar", VK_SPACE);
        create("enter", "Enter", VK_RETURN);
        create("escape", "Escape", VK_ESCAPE);
        create("insert", "Insert", VK_INSERT);
        create("home", "Home", VK_HOME);
        create("pageup", "Page Up", VK_PRIOR);
        create("pagedown", "Page Down", VK_NEXT);
        create("delete", "Delete", VK_DELETE);
        create("end", "End", VK_END);

        create("cancel", "Cancel", VK_CANCEL);
        create("backspace", "Backspace", VK_BACK);
        create("clear", "Clear", VK_CLEAR);
        create("pause", "Pause", VK_PAUSE);
        create("select", "Select", VK_SELECT);
        create("print", "Print", VK_PRINT);
        create("printscreen", "Print Screen", VK_SNAPSHOT);
        create("decimal", ".", VK_DECIMAL);

        create("add", "+", VK_ADD);
        create("subtract", "-", VK_SUBTRACT);
        create("multiply", "*", VK_MULTIPLY);
        create("divide", "/", VK_DIVIDE);

        create("capslock", "Caps Lock", VK_CAPITAL);
        create("numlock", "Num Lock", VK_NUMLOCK);
        create("scrollock", "Scroll Lock", VK_SCROLL);

        create("leftbracket", "[", VK_OEM_4);
        create("rightbracket", "]", VK_OEM_6);

        create("f1", "F1", VK_F1);
        create("f2", "F2", VK_F2);
        create("f3", "F3", VK_F3);
        create("f4", "F4", VK_F4);
        create("f5", "F5", VK_F5);
        create("f6", "F6", VK_F6);
        create("f7", "F7", VK_F7);
        create("f8", "F8", VK_F8);
        create("f9", "F9", VK_F9);
        create("f10", "F10", VK_F10);
        create("f11", "F11", VK_F11);
        create("f12", "F12", VK_F12);
        create("f13", "F13", VK_F13);
        create("f14", "F14", VK_F14);
        create("f15", "F15", VK_F15);
        create("f16", "F16", VK_F16);
        create("f17", "F17", VK_F17);
        create("f18", "F18", VK_F18);
        create("f19", "F19", VK_F19);
        create("f20", "F20", VK_F20);
        create("f21", "F21", VK_F21);
        create("f22", "F22", VK_F22);
        create("f23", "F23", VK_F23);
        create("f24", "F24", VK_F24);

        create("numpad0", "Numpad 0", VK_NUMPAD0);
        create("numpad1", "Numpad 1", VK_NUMPAD1);
        create("numpad2", "Numpad 2", VK_NUMPAD2);
        create("numpad3", "Numpad 3", VK_NUMPAD3);
        create("numpad4", "Numpad 4", VK_NUMPAD4);
        create("numpad5", "Numpad 5", VK_NUMPAD5);
        create("numpad6", "Numpad 6", VK_NUMPAD6);
        create("numpad7", "Numpad 7", VK_NUMPAD7);
        create("numpad8", "Numpad 8", VK_NUMPAD8);
        create("numpad9", "Numpad 9", VK_NUMPAD9);

        create("leftshift", "Left Shift", VK_LSHIFT);
        create("rightshift", "Right Shift", VK_RSHIFT);
        create("leftcontrol", "Left Control", VK_LCONTROL);
        create("rightcontrol", "Right Control", VK_RCONTROL);
        create("leftalt", "Left Alt", VK_LMENU);
        create("rightalt", "Right Alt", VK_RMENU);
        create("leftwindows", "Left Windows", VK_LWIN);
        create("rightwindows", "Right Windows", VK_RWIN);

        return createdInputs;
    }

    ScreenPosition WindowsManager::CurrentMousePosition() const
    {
        POINT point;
        GetCursorPos(&point);

        ScreenToClient(hwnd, &point);
        return { point.x, point.y };
    }

    bool WindowsManager::IsInputDown(Input& input) const
    {
        const auto signalData = input.DataAs<InputDataImplementation>();
        return GetAsyncKeyState(signalData->code) & 0x8000;
    }
}