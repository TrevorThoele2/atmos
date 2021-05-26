#include "SDLInputManager.h"

#include <SDL.h>

namespace Atmos::Input
{
    SDLManager::SDLManager(Logging::Logger& logger) : Manager(logger, "SDL")
    {
        SDL_StartTextInput();

        const auto addMouseButtonMapping = [this](int mouseButton, KeyState KeyStates::* toKeyStates)
        {
            mouseButtonMappings.emplace(mouseButton, KeyMapping{ toKeyStates });
        };

        const auto addKeyCodeMapping = [this](SDL_KeyCode keyCode, KeyState KeyStates::* toKeyStates)
        {
            keycodeMappings.emplace(keyCode, KeyMapping{ toKeyStates });
        };

        addMouseButtonMapping(SDL_BUTTON_LEFT, &KeyStates::leftMouseButton);
        addMouseButtonMapping(SDL_BUTTON_MIDDLE, &KeyStates::middleMouseButton);
        addMouseButtonMapping(SDL_BUTTON_RIGHT, &KeyStates::rightMouseButton);
        addMouseButtonMapping(SDL_BUTTON_X1, &KeyStates::x1MouseButton);
        addMouseButtonMapping(SDL_BUTTON_X2, &KeyStates::x2MouseButton);

        addKeyCodeMapping(SDLK_a, &KeyStates::a);
        addKeyCodeMapping(SDLK_b, &KeyStates::b);
        addKeyCodeMapping(SDLK_c, &KeyStates::c);
        addKeyCodeMapping(SDLK_d, &KeyStates::d);
        addKeyCodeMapping(SDLK_e, &KeyStates::e);
        addKeyCodeMapping(SDLK_f, &KeyStates::f);
        addKeyCodeMapping(SDLK_g, &KeyStates::g);
        addKeyCodeMapping(SDLK_h, &KeyStates::h);
        addKeyCodeMapping(SDLK_i, &KeyStates::i);
        addKeyCodeMapping(SDLK_j, &KeyStates::j);
        addKeyCodeMapping(SDLK_k, &KeyStates::k);
        addKeyCodeMapping(SDLK_l, &KeyStates::l);
        addKeyCodeMapping(SDLK_m, &KeyStates::m);
        addKeyCodeMapping(SDLK_n, &KeyStates::n);
        addKeyCodeMapping(SDLK_o, &KeyStates::o);
        addKeyCodeMapping(SDLK_p, &KeyStates::p);
        addKeyCodeMapping(SDLK_q, &KeyStates::q);
        addKeyCodeMapping(SDLK_r, &KeyStates::r);
        addKeyCodeMapping(SDLK_s, &KeyStates::s);
        addKeyCodeMapping(SDLK_t, &KeyStates::t);
        addKeyCodeMapping(SDLK_u, &KeyStates::u);
        addKeyCodeMapping(SDLK_v, &KeyStates::v);
        addKeyCodeMapping(SDLK_w, &KeyStates::w);
        addKeyCodeMapping(SDLK_x, &KeyStates::x);
        addKeyCodeMapping(SDLK_y, &KeyStates::y);
        addKeyCodeMapping(SDLK_z, &KeyStates::z);

        addKeyCodeMapping(SDLK_0, &KeyStates::zero);
        addKeyCodeMapping(SDLK_1, &KeyStates::one);
        addKeyCodeMapping(SDLK_2, &KeyStates::two);
        addKeyCodeMapping(SDLK_3, &KeyStates::three);
        addKeyCodeMapping(SDLK_4, &KeyStates::four);
        addKeyCodeMapping(SDLK_5, &KeyStates::five);
        addKeyCodeMapping(SDLK_6, &KeyStates::six);
        addKeyCodeMapping(SDLK_7, &KeyStates::seven);
        addKeyCodeMapping(SDLK_8, &KeyStates::eight);
        addKeyCodeMapping(SDLK_9, &KeyStates::nine);

        addKeyCodeMapping(SDLK_LEFT, &KeyStates::left);
        addKeyCodeMapping(SDLK_UP, &KeyStates::up);
        addKeyCodeMapping(SDLK_RIGHT, &KeyStates::right);
        addKeyCodeMapping(SDLK_DOWN, &KeyStates::down);

        addKeyCodeMapping(SDLK_F1, &KeyStates::f1);
        addKeyCodeMapping(SDLK_F2, &KeyStates::f2);
        addKeyCodeMapping(SDLK_F3, &KeyStates::f3);
        addKeyCodeMapping(SDLK_F4, &KeyStates::f4);
        addKeyCodeMapping(SDLK_F5, &KeyStates::f5);
        addKeyCodeMapping(SDLK_F6, &KeyStates::f6);
        addKeyCodeMapping(SDLK_F7, &KeyStates::f7);
        addKeyCodeMapping(SDLK_F8, &KeyStates::f8);
        addKeyCodeMapping(SDLK_F9, &KeyStates::f9);
        addKeyCodeMapping(SDLK_F10, &KeyStates::f10);
        addKeyCodeMapping(SDLK_F11, &KeyStates::f11);
        addKeyCodeMapping(SDLK_F12, &KeyStates::f12);
        addKeyCodeMapping(SDLK_F13, &KeyStates::f13);
        addKeyCodeMapping(SDLK_F14, &KeyStates::f14);
        addKeyCodeMapping(SDLK_F15, &KeyStates::f15);
        addKeyCodeMapping(SDLK_F16, &KeyStates::f16);
        addKeyCodeMapping(SDLK_F17, &KeyStates::f17);
        addKeyCodeMapping(SDLK_F18, &KeyStates::f18);
        addKeyCodeMapping(SDLK_F19, &KeyStates::f19);
        addKeyCodeMapping(SDLK_F20, &KeyStates::f20);
        addKeyCodeMapping(SDLK_F21, &KeyStates::f21);
        addKeyCodeMapping(SDLK_F22, &KeyStates::f22);
        addKeyCodeMapping(SDLK_F23, &KeyStates::f23);
        addKeyCodeMapping(SDLK_F24, &KeyStates::f24);

        addKeyCodeMapping(SDLK_LALT, &KeyStates::leftAlt);
        addKeyCodeMapping(SDLK_LCTRL, &KeyStates::leftControl);
        addKeyCodeMapping(SDLK_LSHIFT, &KeyStates::leftShift);
        addKeyCodeMapping(SDLK_LEFTBRACKET, &KeyStates::leftBracket);
        addKeyCodeMapping(SDLK_LGUI, &KeyStates::leftGui);

        addKeyCodeMapping(SDLK_RALT, &KeyStates::rightAlt);
        addKeyCodeMapping(SDLK_RCTRL, &KeyStates::rightControl);
        addKeyCodeMapping(SDLK_RSHIFT, &KeyStates::rightShift);
        addKeyCodeMapping(SDLK_RIGHTBRACKET, &KeyStates::rightBracket);
        addKeyCodeMapping(SDLK_RGUI, &KeyStates::rightGui);

        addKeyCodeMapping(SDLK_PAGEDOWN, &KeyStates::pageDown);
        addKeyCodeMapping(SDLK_PAGEUP, &KeyStates::pageUp);

        addKeyCodeMapping(SDLK_KP_0, &KeyStates::keypad0);
        addKeyCodeMapping(SDLK_KP_00, &KeyStates::keypad00);
        addKeyCodeMapping(SDLK_KP_000, &KeyStates::keypad000);
        addKeyCodeMapping(SDLK_KP_1, &KeyStates::keypad1);
        addKeyCodeMapping(SDLK_KP_2, &KeyStates::keypad2);
        addKeyCodeMapping(SDLK_KP_3, &KeyStates::keypad3);
        addKeyCodeMapping(SDLK_KP_4, &KeyStates::keypad4);
        addKeyCodeMapping(SDLK_KP_5, &KeyStates::keypad5);
        addKeyCodeMapping(SDLK_KP_6, &KeyStates::keypad6);
        addKeyCodeMapping(SDLK_KP_7, &KeyStates::keypad7);
        addKeyCodeMapping(SDLK_KP_8, &KeyStates::keypad8);
        addKeyCodeMapping(SDLK_KP_9, &KeyStates::keypad9);
        addKeyCodeMapping(SDLK_KP_A, &KeyStates::keypadA);
        addKeyCodeMapping(SDLK_KP_B, &KeyStates::keypadB);
        addKeyCodeMapping(SDLK_KP_C, &KeyStates::keypadC);
        addKeyCodeMapping(SDLK_KP_D, &KeyStates::keypadD);
        addKeyCodeMapping(SDLK_KP_E, &KeyStates::keypadE);
        addKeyCodeMapping(SDLK_KP_F, &KeyStates::keypadF);
        addKeyCodeMapping(SDLK_KP_AMPERSAND, &KeyStates::keypadAmpersand);
        addKeyCodeMapping(SDLK_KP_AT, &KeyStates::keypadAt);
        addKeyCodeMapping(SDLK_KP_BACKSPACE, &KeyStates::keypadBackspace);
        addKeyCodeMapping(SDLK_KP_BINARY, &KeyStates::keypadBinary);
        addKeyCodeMapping(SDLK_KP_CLEAR, &KeyStates::keypadClear);
        addKeyCodeMapping(SDLK_KP_CLEARENTRY, &KeyStates::keypadClearEntry);
        addKeyCodeMapping(SDLK_KP_COLON, &KeyStates::keypadColon);
        addKeyCodeMapping(SDLK_KP_COMMA, &KeyStates::keypadComma);
        addKeyCodeMapping(SDLK_KP_DBLAMPERSAND, &KeyStates::keypadAmpersandAmpersand);
        addKeyCodeMapping(SDLK_KP_DBLVERTICALBAR, &KeyStates::keypadBarBar);
        addKeyCodeMapping(SDLK_KP_DECIMAL, &KeyStates::keypadDecimal);
        addKeyCodeMapping(SDLK_KP_DIVIDE, &KeyStates::keypadForwardSlash);
        addKeyCodeMapping(SDLK_KP_ENTER, &KeyStates::keypadEnter);
        addKeyCodeMapping(SDLK_KP_EQUALS, &KeyStates::keypadEquals);
        addKeyCodeMapping(SDLK_KP_EQUALSAS400, &KeyStates::keypadEqualsAS400);
        addKeyCodeMapping(SDLK_KP_EXCLAM, &KeyStates::keypadExclamation);
        addKeyCodeMapping(SDLK_KP_GREATER, &KeyStates::keypadGreater);
        addKeyCodeMapping(SDLK_KP_LESS, &KeyStates::keypadLess);
        addKeyCodeMapping(SDLK_KP_HASH, &KeyStates::keypadPound);
        addKeyCodeMapping(SDLK_KP_HEXADECIMAL, &KeyStates::keypadHexadecimal);
        addKeyCodeMapping(SDLK_KP_LEFTBRACE, &KeyStates::keypadLeftBrace);
        addKeyCodeMapping(SDLK_KP_LEFTPAREN, &KeyStates::keypadLeftParenthesis);
        addKeyCodeMapping(SDLK_KP_MEMADD, &KeyStates::keypadMemAdd);
        addKeyCodeMapping(SDLK_KP_MEMCLEAR, &KeyStates::keypadMemClear);
        addKeyCodeMapping(SDLK_KP_MEMDIVIDE, &KeyStates::keypadMemDivide);
        addKeyCodeMapping(SDLK_KP_MEMMULTIPLY, &KeyStates::keypadMemMultiply);
        addKeyCodeMapping(SDLK_KP_MEMCLEAR, &KeyStates::keypadMemRecall);
        addKeyCodeMapping(SDLK_KP_MEMSTORE, &KeyStates::keypadMemStore);
        addKeyCodeMapping(SDLK_KP_MEMSUBTRACT, &KeyStates::keypadMemSubtract);
        addKeyCodeMapping(SDLK_KP_MINUS, &KeyStates::keypadHyphen);
        addKeyCodeMapping(SDLK_KP_MULTIPLY, &KeyStates::keypadAsterisk);
        addKeyCodeMapping(SDLK_KP_OCTAL, &KeyStates::keypadOctal);
        addKeyCodeMapping(SDLK_KP_PERCENT, &KeyStates::keypadPercent);
        addKeyCodeMapping(SDLK_KP_PERIOD, &KeyStates::keypadPeriod);
        addKeyCodeMapping(SDLK_KP_PLUS, &KeyStates::keypadPlus);
        addKeyCodeMapping(SDLK_KP_PLUSMINUS, &KeyStates::keypadPlusMinus);
        addKeyCodeMapping(SDLK_KP_POWER, &KeyStates::keypadPower);
        addKeyCodeMapping(SDLK_KP_RIGHTBRACE, &KeyStates::keypadRightBrace);
        addKeyCodeMapping(SDLK_KP_RIGHTPAREN, &KeyStates::keypadRightParenthesis);
        addKeyCodeMapping(SDLK_KP_SPACE, &KeyStates::keypadSpace);
        addKeyCodeMapping(SDLK_KP_TAB, &KeyStates::keypadTab);
        addKeyCodeMapping(SDLK_KP_VERTICALBAR, &KeyStates::keypadBar);
        addKeyCodeMapping(SDLK_KP_XOR, &KeyStates::keypadXor);

        addKeyCodeMapping(SDLK_BRIGHTNESSUP, &KeyStates::brightnessUp);
        addKeyCodeMapping(SDLK_BRIGHTNESSDOWN, &KeyStates::brightnessDown);

        addKeyCodeMapping(SDLK_VOLUMEUP, &KeyStates::volumeUp);
        addKeyCodeMapping(SDLK_VOLUMEDOWN, &KeyStates::volumeDown);

        addKeyCodeMapping(SDLK_AUDIOMUTE, &KeyStates::audioMute);
        addKeyCodeMapping(SDLK_AUDIONEXT, &KeyStates::audioNext);
        addKeyCodeMapping(SDLK_AUDIOPLAY, &KeyStates::audioPlay);
        addKeyCodeMapping(SDLK_AUDIOPREV, &KeyStates::audioPrevious);
        addKeyCodeMapping(SDLK_AUDIOSTOP, &KeyStates::audioStop);

        addKeyCodeMapping(SDLK_AC_BACK, &KeyStates::acBack);
        addKeyCodeMapping(SDLK_AC_BOOKMARKS, &KeyStates::acBookmarks);
        addKeyCodeMapping(SDLK_AC_FORWARD, &KeyStates::acForward);
        addKeyCodeMapping(SDLK_AC_HOME, &KeyStates::acHome);
        addKeyCodeMapping(SDLK_AC_REFRESH, &KeyStates::acRefresh);
        addKeyCodeMapping(SDLK_AC_SEARCH, &KeyStates::acSearch);
        addKeyCodeMapping(SDLK_AC_STOP, &KeyStates::acStop);

        addKeyCodeMapping(SDLK_KBDILLUMDOWN, &KeyStates::illuminationDown);
        addKeyCodeMapping(SDLK_KBDILLUMUP, &KeyStates::illuminationUp);
        addKeyCodeMapping(SDLK_KBDILLUMTOGGLE, &KeyStates::illuminationToggle);

        addKeyCodeMapping(SDLK_BACKSLASH, &KeyStates::backslash);
        addKeyCodeMapping(SDLK_BACKSPACE, &KeyStates::backspace);
        addKeyCodeMapping(SDLK_COMMA, &KeyStates::comma);
        addKeyCodeMapping(SDLK_QUOTE, &KeyStates::quote);
        addKeyCodeMapping(SDLK_AGAIN, &KeyStates::again);
        addKeyCodeMapping(SDLK_ALTERASE, &KeyStates::altErase);
        addKeyCodeMapping(SDLK_APPLICATION, &KeyStates::application);
        addKeyCodeMapping(SDLK_CALCULATOR, &KeyStates::calculator);
        addKeyCodeMapping(SDLK_CANCEL, &KeyStates::cancel);
        addKeyCodeMapping(SDLK_CAPSLOCK, &KeyStates::capsLock);
        addKeyCodeMapping(SDLK_CLEAR, &KeyStates::clear);
        addKeyCodeMapping(SDLK_CLEARAGAIN, &KeyStates::clearAgain);
        addKeyCodeMapping(SDLK_COMPUTER, &KeyStates::computer);
        addKeyCodeMapping(SDLK_COPY, &KeyStates::copy);
        addKeyCodeMapping(SDLK_CRSEL, &KeyStates::crSel);
        addKeyCodeMapping(SDLK_CURRENCYSUBUNIT, &KeyStates::currencySubUnit);
        addKeyCodeMapping(SDLK_CURRENCYUNIT, &KeyStates::currencyUnit);
        addKeyCodeMapping(SDLK_CUT, &KeyStates::cut);
        addKeyCodeMapping(SDLK_DECIMALSEPARATOR, &KeyStates::decimalSeparator);
        addKeyCodeMapping(SDLK_DELETE, &KeyStates::del);
        addKeyCodeMapping(SDLK_DISPLAYSWITCH, &KeyStates::displaySwitch);
        addKeyCodeMapping(SDLK_EJECT, &KeyStates::eject);
        addKeyCodeMapping(SDLK_END, &KeyStates::end);
        addKeyCodeMapping(SDLK_EQUALS, &KeyStates::equals);
        addKeyCodeMapping(SDLK_ESCAPE, &KeyStates::escape);
        addKeyCodeMapping(SDLK_EXECUTE, &KeyStates::execute);
        addKeyCodeMapping(SDLK_EXSEL, &KeyStates::exSel);
        addKeyCodeMapping(SDLK_FIND, &KeyStates::find);
        addKeyCodeMapping(SDLK_BACKQUOTE, &KeyStates::grave);
        addKeyCodeMapping(SDLK_HELP, &KeyStates::help);
        addKeyCodeMapping(SDLK_HOME, &KeyStates::home);
        addKeyCodeMapping(SDLK_INSERT, &KeyStates::insert);
        addKeyCodeMapping(SDLK_MAIL, &KeyStates::mail);
        addKeyCodeMapping(SDLK_MEDIASELECT, &KeyStates::mediaSelect);
        addKeyCodeMapping(SDLK_MENU, &KeyStates::menu);
        addKeyCodeMapping(SDLK_MINUS, &KeyStates::hyphen);
        addKeyCodeMapping(SDLK_MODE, &KeyStates::modeSwitch);
        addKeyCodeMapping(SDLK_MUTE, &KeyStates::mute);
        addKeyCodeMapping(SDLK_NUMLOCKCLEAR, &KeyStates::numlock);
        addKeyCodeMapping(SDLK_OPER, &KeyStates::oper);
        addKeyCodeMapping(SDLK_OUT, &KeyStates::out);
        addKeyCodeMapping(SDLK_PASTE, &KeyStates::paste);
        addKeyCodeMapping(SDLK_PAUSE, &KeyStates::pause);
        addKeyCodeMapping(SDLK_PERIOD, &KeyStates::period);
        addKeyCodeMapping(SDLK_POWER, &KeyStates::power);
        addKeyCodeMapping(SDLK_PRINTSCREEN, &KeyStates::printScreen);
        addKeyCodeMapping(SDLK_PRIOR, &KeyStates::prior);
        addKeyCodeMapping(SDLK_RETURN, &KeyStates::enter);
        addKeyCodeMapping(SDLK_SCROLLLOCK, &KeyStates::scrollLock);
        addKeyCodeMapping(SDLK_SELECT, &KeyStates::select);
        addKeyCodeMapping(SDLK_SEMICOLON, &KeyStates::semicolon);
        addKeyCodeMapping(SDLK_SEPARATOR, &KeyStates::separator);
        addKeyCodeMapping(SDLK_SLASH, &KeyStates::forwardSlash);
        addKeyCodeMapping(SDLK_SLEEP, &KeyStates::sleep);
        addKeyCodeMapping(SDLK_SPACE, &KeyStates::space);
        addKeyCodeMapping(SDLK_STOP, &KeyStates::stop);
        addKeyCodeMapping(SDLK_SYSREQ, &KeyStates::sysReq);
        addKeyCodeMapping(SDLK_TAB, &KeyStates::tab);
        addKeyCodeMapping(SDLK_THOUSANDSSEPARATOR, &KeyStates::thousandsSeparator);
        addKeyCodeMapping(SDLK_UNDO, &KeyStates::undo);
        addKeyCodeMapping(SDLK_WWW, &KeyStates::worldWideWeb);
    }

    auto SDLManager::ReadState() const -> State
    {
        State state;

        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_TEXTINPUT:
                HandleTextInputEvent(event.text, state);
                break;
            default:
                break;
            }
        }

        HandleMouse(state);
        HandleKeyboard(state);

        return state;
    }

    void SDLManager::HandleTextInputEvent(SDL_TextInputEvent event, State& state) const
    {
        state.enteredText.emplace_back(event.text);
    }

    void SDLManager::HandleMouse(State& state) const
    {
        int x;
        int y;
        const auto mouseState = SDL_GetMouseState(&x, &y);

        state.mousePosition.x = static_cast<Spatial::Point2D::Value>(x);
        state.mousePosition.y = static_cast<Spatial::Point2D::Value>(y);

        for(auto& [mouseButton, mapping] : mouseButtonMappings)
        {
            const auto isDown = mouseState & SDL_BUTTON(mouseButton);
            state.keyStates.*mapping.toKeyStates = isDown ? KeyState::Down : KeyState::Up;
        }
    }

    void SDLManager::HandleKeyboard(State& state) const
    {
        const auto keyboardState = SDL_GetKeyboardState(nullptr);

        for(auto& [keyCode, mapping] : keycodeMappings)
        {
            const auto isDown = keyboardState[SDL_GetScancodeFromKey(keyCode)];
            state.keyStates.*mapping.toKeyStates = isDown ? KeyState::Down : KeyState::Up;
        }
    }

    auto SDLManager::FromMouse(int mouseButton) const -> const KeyMapping*
    {
        const auto found = mouseButtonMappings.find(mouseButton);
        if (found == mouseButtonMappings.end())
            return nullptr;
        else
            return &found->second;
    }

    auto SDLManager::FromKeycode(SDL_Keycode keycode) const -> const KeyMapping*
    {
        const auto found = keycodeMappings.find(keycode);
        if (found == keycodeMappings.end())
            return nullptr;
        else
            return &found->second;
    }
}
