
#include "WindowsInputManager.h"

#include "WindowsInclude.h"

#include "ObjectManager.h"
#include "WindowSystem.h"

#include "WindowsWindow.h"

#include "IsSameRuntimeType.h"

namespace Atmos::Input
{
    class SignalBaseData : public SignalBase::Data
    {
    public:
        typedef int Code;
        Code code;
    public:
        SignalBaseData(Code code) : code(code)
        {}
    };

    void WindowsManager::CreateSignalsImpl()
    {
        auto createSignal = [](int code) -> SignalDataPtr
        {
            return SignalDataPtr(new SignalBaseData(code));
        };

        CreateMouseKey<MouseKeyID::LEFT>(createSignal(VK_LBUTTON));
        CreateMouseKey<MouseKeyID::RIGHT>(createSignal(VK_RBUTTON));

        // Inputs
        CreateKey<KeyID::A>(createSignal(0x41));
        CreateKey<KeyID::B>(createSignal(0x42));
        CreateKey<KeyID::C>(createSignal(0x43));
        CreateKey<KeyID::D>(createSignal(0x44));
        CreateKey<KeyID::E>(createSignal(0x45));
        CreateKey<KeyID::F>(createSignal(0x46));
        CreateKey<KeyID::G>(createSignal(0x47));
        CreateKey<KeyID::H>(createSignal(0x48));
        CreateKey<KeyID::I>(createSignal(0x49));
        CreateKey<KeyID::J>(createSignal(0x4A));
        CreateKey<KeyID::K>(createSignal(0x4B));
        CreateKey<KeyID::L>(createSignal(0x4C));
        CreateKey<KeyID::M>(createSignal(0x4D));
        CreateKey<KeyID::N>(createSignal(0x4E));
        CreateKey<KeyID::O>(createSignal(0x4F));
        CreateKey<KeyID::P>(createSignal(0x50));
        CreateKey<KeyID::Q>(createSignal(0x51));
        CreateKey<KeyID::R>(createSignal(0x52));
        CreateKey<KeyID::S>(createSignal(0x53));
        CreateKey<KeyID::T>(createSignal(0x54));
        CreateKey<KeyID::U>(createSignal(0x55));
        CreateKey<KeyID::V>(createSignal(0x56));
        CreateKey<KeyID::W>(createSignal(0x57));
        CreateKey<KeyID::X>(createSignal(0x58));
        CreateKey<KeyID::Y>(createSignal(0x59));
        CreateKey<KeyID::Z>(createSignal(0x5A));

        CreateKey<KeyID::LEFT_ARROW>(createSignal(VK_LEFT));
        CreateKey<KeyID::UP_ARROW>(createSignal(VK_UP));
        CreateKey<KeyID::RIGHT_ARROW>(createSignal(VK_RIGHT));
        CreateKey<KeyID::DOWN_ARROW>(createSignal(VK_DOWN));

        CreateKey<KeyID::TAB>(createSignal(VK_TAB));
        CreateKey<KeyID::SPACEBAR>(createSignal(VK_SPACE));
        CreateKey<KeyID::ENTER>(createSignal(VK_RETURN));
        CreateKey<KeyID::ESCAPE>(createSignal(VK_ESCAPE));
        CreateKey<KeyID::INSERT>(createSignal(VK_INSERT));
        CreateKey<KeyID::HOME>(createSignal(VK_HOME));
        CreateKey<KeyID::PAGE_UP>(createSignal(VK_PRIOR));
        CreateKey<KeyID::PAGE_DOWN>(createSignal(VK_NEXT));
        CreateKey<KeyID::DEL>(createSignal(VK_DELETE));
        CreateKey<KeyID::END>(createSignal(VK_END));

        CreateKey<KeyID::CANCEL>(createSignal(VK_CANCEL));
        CreateKey<KeyID::BACKSPACE>(createSignal(VK_BACK));
        CreateKey<KeyID::CLEAR>(createSignal(VK_CLEAR));
        CreateKey<KeyID::PAUSE>(createSignal(VK_PAUSE));
        CreateKey<KeyID::SELECT>(createSignal(VK_SELECT));
        CreateKey<KeyID::PRINT>(createSignal(VK_PRINT));
        CreateKey<KeyID::PRINT_SCREEN>(createSignal(VK_SNAPSHOT));
        CreateKey<KeyID::DECIMAL>(createSignal(VK_DECIMAL));

        CreateKey<KeyID::ADD>(createSignal(VK_ADD));
        CreateKey<KeyID::SUBTRACT>(createSignal(VK_SUBTRACT));
        CreateKey<KeyID::MULTIPLY>(createSignal(VK_MULTIPLY));
        CreateKey<KeyID::DIVIDE>(createSignal(VK_DIVIDE));

        CreateKey<KeyID::CAPS_LOCK>(createSignal(VK_CAPITAL));
        CreateKey<KeyID::NUM_LOCK>(createSignal(VK_NUMLOCK));
        CreateKey<KeyID::SCROLL_LOCK>(createSignal(VK_SCROLL));

        CreateKey<KeyID::L_BRACKET>(createSignal(VK_OEM_4));
        CreateKey<KeyID::R_BRACKET>(createSignal(VK_OEM_6));

        CreateKey<KeyID::F1>(createSignal(VK_F1));
        CreateKey<KeyID::F2>(createSignal(VK_F2));
        CreateKey<KeyID::F3>(createSignal(VK_F3));
        CreateKey<KeyID::F4>(createSignal(VK_F4));
        CreateKey<KeyID::F5>(createSignal(VK_F5));
        CreateKey<KeyID::F6>(createSignal(VK_F6));
        CreateKey<KeyID::F7>(createSignal(VK_F7));
        CreateKey<KeyID::F8>(createSignal(VK_F8));
        CreateKey<KeyID::F9>(createSignal(VK_F9));
        CreateKey<KeyID::F10>(createSignal(VK_F10));
        CreateKey<KeyID::F11>(createSignal(VK_F11));
        CreateKey<KeyID::F12>(createSignal(VK_F12));
        CreateKey<KeyID::F13>(createSignal(VK_F13));
        CreateKey<KeyID::F14>(createSignal(VK_F14));
        CreateKey<KeyID::F15>(createSignal(VK_F15));
        CreateKey<KeyID::F16>(createSignal(VK_F16));
        CreateKey<KeyID::F17>(createSignal(VK_F17));
        CreateKey<KeyID::F18>(createSignal(VK_F18));
        CreateKey<KeyID::F19>(createSignal(VK_F19));
        CreateKey<KeyID::F20>(createSignal(VK_F20));
        CreateKey<KeyID::F21>(createSignal(VK_F21));
        CreateKey<KeyID::F22>(createSignal(VK_F22));
        CreateKey<KeyID::F23>(createSignal(VK_F23));
        CreateKey<KeyID::F24>(createSignal(VK_F24));

        CreateKey<KeyID::NUMPAD_0>(createSignal(VK_NUMPAD0));
        CreateKey<KeyID::NUMPAD_1>(createSignal(VK_NUMPAD1));
        CreateKey<KeyID::NUMPAD_2>(createSignal(VK_NUMPAD2));
        CreateKey<KeyID::NUMPAD_3>(createSignal(VK_NUMPAD3));
        CreateKey<KeyID::NUMPAD_4>(createSignal(VK_NUMPAD4));
        CreateKey<KeyID::NUMPAD_5>(createSignal(VK_NUMPAD5));
        CreateKey<KeyID::NUMPAD_6>(createSignal(VK_NUMPAD6));
        CreateKey<KeyID::NUMPAD_7>(createSignal(VK_NUMPAD7));
        CreateKey<KeyID::NUMPAD_8>(createSignal(VK_NUMPAD8));
        CreateKey<KeyID::NUMPAD_9>(createSignal(VK_NUMPAD9));

        CreateKey<KeyID::L_SHIFT>(createSignal(VK_LSHIFT));
        CreateKey<KeyID::R_SHIFT>(createSignal(VK_RSHIFT));
        CreateKey<KeyID::L_CONTROL>(createSignal(VK_LCONTROL));
        CreateKey<KeyID::R_CONTROL>(createSignal(VK_RCONTROL));
        CreateKey<KeyID::L_ALT>(createSignal(VK_LMENU));
        CreateKey<KeyID::R_ALT>(createSignal(VK_RMENU));
        CreateKey<KeyID::L_WIN>(createSignal(VK_LWIN));
        CreateKey<KeyID::R_WIN>(createSignal(VK_RWIN));
    }

    void WindowsManager::WorkSignalsImpl(SignalList& signalList)
    {
        for (auto& loop : signalList)
            loop->Work(IsSignalDownBase(*loop));
    }

    bool WindowsManager::ShouldAddActions() const
    {
        return true;
    }

    MousePosition WindowsManager::GetMousePositionImpl() const
    {
        POINT pt;
        GetCursorPos(&pt);

        auto windowSystem = FindWindowSystem();
        auto window = static_cast<Window::WindowsWindowBase*>(windowSystem->Get());
        ScreenToClient(window->GetHwnd(), &pt);
        return MousePosition(static_cast<MousePosition::Value>(pt.x), static_cast<MousePosition::Value>(pt.y));
    }

    bool WindowsManager::IsSignalDownBase(SignalBase &signal) const
    {
        auto isMouseKey = IsSameRuntimeType<MouseKey>(signal);
        if (isMouseKey && !IsMouseWithinScreen())
            return false;

        auto signalData = signal.GetData<SignalBaseData>();
        return GetAsyncKeyState(signalData->code) & 0x8000 ? 1 : 0;
    }

    bool WindowsManager::IsMouseWithinScreen() const
    {
        auto windowSystem = FindWindowSystem();
        auto& clientSize = windowSystem->Get()->ClientSize();
        AxisAlignedBox2D screen(
            0,
            0,
            static_cast<AxisAlignedBox2D::Coordinate>(clientSize.width),
            static_cast<AxisAlignedBox2D::Coordinate>(clientSize.height));
        return screen.IsHit(GetMousePositionImpl());
    }

    Window::WindowSystem* WindowsManager::FindWindowSystem() const
    {
        return GetObjectManager()->FindSystem<Window::WindowSystem>();
    }
}