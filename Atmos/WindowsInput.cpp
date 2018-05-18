
#include "WindowsInput.h"
#include "Input.h"
#include "Environment.h"
#include "Windows.h"

#include "WindowsInclude.h"

namespace Atmos
{
    namespace Input
    {
        bool IsKeyDownBase(int code)
        {
            return GetAsyncKeyState(code) & 0x8000 ? 1 : 0;
        }

        class SignalBaseData : public SignalBase::Data
        {
        public:
            typedef int Code;
            Code code;

            SignalBaseData(Code code) : code(code)
            {}
        };

        void WindowsHandler::AddInputsImpl()
        {
            AddMouseInput<MouseButton::LEFT>(new SignalBaseData(VK_LBUTTON));
            AddMouseInput<MouseButton::RIGHT>(new SignalBaseData(VK_RBUTTON));

            // Inputs
            AddInput<KeyID::A>(new SignalBaseData(0x41));
            AddInput<KeyID::B>(new SignalBaseData(0x42));
            AddInput<KeyID::C>(new SignalBaseData(0x43));
            AddInput<KeyID::D>(new SignalBaseData(0x44));
            AddInput<KeyID::E>(new SignalBaseData(0x45));
            AddInput<KeyID::F>(new SignalBaseData(0x46));
            AddInput<KeyID::G>(new SignalBaseData(0x47));
            AddInput<KeyID::H>(new SignalBaseData(0x48));
            AddInput<KeyID::I>(new SignalBaseData(0x49));
            AddInput<KeyID::J>(new SignalBaseData(0x4A));
            AddInput<KeyID::K>(new SignalBaseData(0x4B));
            AddInput<KeyID::L>(new SignalBaseData(0x4C));
            AddInput<KeyID::M>(new SignalBaseData(0x4D));
            AddInput<KeyID::N>(new SignalBaseData(0x4E));
            AddInput<KeyID::O>(new SignalBaseData(0x4F));
            AddInput<KeyID::P>(new SignalBaseData(0x50));
            AddInput<KeyID::Q>(new SignalBaseData(0x51));
            AddInput<KeyID::R>(new SignalBaseData(0x52));
            AddInput<KeyID::S>(new SignalBaseData(0x53));
            AddInput<KeyID::T>(new SignalBaseData(0x54));
            AddInput<KeyID::U>(new SignalBaseData(0x55));
            AddInput<KeyID::V>(new SignalBaseData(0x56));
            AddInput<KeyID::W>(new SignalBaseData(0x57));
            AddInput<KeyID::X>(new SignalBaseData(0x58));
            AddInput<KeyID::Y>(new SignalBaseData(0x59));
            AddInput<KeyID::Z>(new SignalBaseData(0x5A));

            AddInput<KeyID::LEFT_ARROW>(new SignalBaseData(VK_LEFT));
            AddInput<KeyID::UP_ARROW>(new SignalBaseData(VK_UP));
            AddInput<KeyID::RIGHT_ARROW>(new SignalBaseData(VK_RIGHT));
            AddInput<KeyID::DOWN_ARROW>(new SignalBaseData(VK_DOWN));

            AddInput<KeyID::TAB>(new SignalBaseData(VK_TAB));
            AddInput<KeyID::SPACEBAR>(new SignalBaseData(VK_SPACE));
            AddInput<KeyID::ENTER>(new SignalBaseData(VK_RETURN));
            AddInput<KeyID::ESCAPE>(new SignalBaseData(VK_ESCAPE));
            AddInput<KeyID::INSERT>(new SignalBaseData(VK_INSERT));
            AddInput<KeyID::HOME>(new SignalBaseData(VK_HOME));
            AddInput<KeyID::PAGE_UP>(new SignalBaseData(VK_PRIOR));
            AddInput<KeyID::PAGE_DOWN>(new SignalBaseData(VK_NEXT));
            AddInput<KeyID::DEL>(new SignalBaseData(VK_DELETE));
            AddInput<KeyID::END>(new SignalBaseData(VK_END));

            AddInput<KeyID::CANCEL>(new SignalBaseData(VK_CANCEL));
            AddInput<KeyID::BACKSPACE>(new SignalBaseData(VK_BACK));
            AddInput<KeyID::CLEAR>(new SignalBaseData(VK_CLEAR));
            AddInput<KeyID::PAUSE>(new SignalBaseData(VK_PAUSE));
            AddInput<KeyID::SELECT>(new SignalBaseData(VK_SELECT));
            AddInput<KeyID::PRINT>(new SignalBaseData(VK_PRINT));
            AddInput<KeyID::PRINT_SCREEN>(new SignalBaseData(VK_SNAPSHOT));
            AddInput<KeyID::DECIMAL>(new SignalBaseData(VK_DECIMAL));

            AddInput<KeyID::ADD>(new SignalBaseData(VK_ADD));
            AddInput<KeyID::SUBTRACT>(new SignalBaseData(VK_SUBTRACT));
            AddInput<KeyID::MULTIPLY>(new SignalBaseData(VK_MULTIPLY));
            AddInput<KeyID::DIVIDE>(new SignalBaseData(VK_DIVIDE));

            AddInput<KeyID::CAPS_LOCK>(new SignalBaseData(VK_CAPITAL));
            AddInput<KeyID::NUM_LOCK>(new SignalBaseData(VK_NUMLOCK));
            AddInput<KeyID::SCROLL_LOCK>(new SignalBaseData(VK_SCROLL));

            AddInput<KeyID::L_BRACKET>(new SignalBaseData(VK_OEM_4));
            AddInput<KeyID::R_BRACKET>(new SignalBaseData(VK_OEM_6));

            AddInput<KeyID::F1>(new SignalBaseData(VK_F1));
            AddInput<KeyID::F2>(new SignalBaseData(VK_F2));
            AddInput<KeyID::F3>(new SignalBaseData(VK_F3));
            AddInput<KeyID::F4>(new SignalBaseData(VK_F4));
            AddInput<KeyID::F5>(new SignalBaseData(VK_F5));
            AddInput<KeyID::F6>(new SignalBaseData(VK_F6));
            AddInput<KeyID::F7>(new SignalBaseData(VK_F7));
            AddInput<KeyID::F8>(new SignalBaseData(VK_F8));
            AddInput<KeyID::F9>(new SignalBaseData(VK_F9));
            AddInput<KeyID::F10>(new SignalBaseData(VK_F10));
            AddInput<KeyID::F11>(new SignalBaseData(VK_F11));
            AddInput<KeyID::F12>(new SignalBaseData(VK_F12));
            AddInput<KeyID::F13>(new SignalBaseData(VK_F13));
            AddInput<KeyID::F14>(new SignalBaseData(VK_F14));
            AddInput<KeyID::F15>(new SignalBaseData(VK_F15));
            AddInput<KeyID::F16>(new SignalBaseData(VK_F16));
            AddInput<KeyID::F17>(new SignalBaseData(VK_F17));
            AddInput<KeyID::F18>(new SignalBaseData(VK_F18));
            AddInput<KeyID::F19>(new SignalBaseData(VK_F19));
            AddInput<KeyID::F20>(new SignalBaseData(VK_F20));
            AddInput<KeyID::F21>(new SignalBaseData(VK_F21));
            AddInput<KeyID::F22>(new SignalBaseData(VK_F22));
            AddInput<KeyID::F23>(new SignalBaseData(VK_F23));
            AddInput<KeyID::F24>(new SignalBaseData(VK_F24));

            AddInput<KeyID::NUMPAD_0>(new SignalBaseData(VK_NUMPAD0));
            AddInput<KeyID::NUMPAD_1>(new SignalBaseData(VK_NUMPAD1));
            AddInput<KeyID::NUMPAD_2>(new SignalBaseData(VK_NUMPAD2));
            AddInput<KeyID::NUMPAD_3>(new SignalBaseData(VK_NUMPAD3));
            AddInput<KeyID::NUMPAD_4>(new SignalBaseData(VK_NUMPAD4));
            AddInput<KeyID::NUMPAD_5>(new SignalBaseData(VK_NUMPAD5));
            AddInput<KeyID::NUMPAD_6>(new SignalBaseData(VK_NUMPAD6));
            AddInput<KeyID::NUMPAD_7>(new SignalBaseData(VK_NUMPAD7));
            AddInput<KeyID::NUMPAD_8>(new SignalBaseData(VK_NUMPAD8));
            AddInput<KeyID::NUMPAD_9>(new SignalBaseData(VK_NUMPAD9));

            AddInput<KeyID::L_SHIFT>(new SignalBaseData(VK_LSHIFT));
            AddInput<KeyID::R_SHIFT>(new SignalBaseData(VK_RSHIFT));
            AddInput<KeyID::L_CONTROL>(new SignalBaseData(VK_LCONTROL));
            AddInput<KeyID::R_CONTROL>(new SignalBaseData(VK_RCONTROL));
            AddInput<KeyID::L_ALT>(new SignalBaseData(VK_LMENU));
            AddInput<KeyID::R_ALT>(new SignalBaseData(VK_RMENU));
            AddInput<KeyID::L_WIN>(new SignalBaseData(VK_LWIN));
            AddInput<KeyID::R_WIN>(new SignalBaseData(VK_RWIN));
        }

        void WindowsHandler::WorkInputsImpl(InputVector &inputs)
        {
            for (auto &loop : inputs)
                loop->Work(IsKeyDownBase(loop->GetData<SignalBaseData>()->code));
        }

        bool WindowsHandler::ShouldAddActions() const
        {
            return true;
        }

        MousePosition WindowsHandler::GetMousePositionImpl() const
        {
            POINT pt;
            GetCursorPos(&pt);
            ScreenToClient(Environment::GetModel<Windows>()->GetHwnd(), &pt);
            return MousePosition(static_cast<MousePosition::ValueT>(pt.x), static_cast<MousePosition::ValueT>(pt.y));
        }
    }
}