#include "InputCurator.h"

#include "ActionActive.h"
#include "ActionPressed.h"
#include "ActionDepressed.h"
#include "MouseMoved.h"
#include "TextEntered.h"
#include "Log.h"

namespace Atmos::Input
{
    Curator::Curator(Init init, Manager& manager) :
        Arca::Curator(init), manager(&manager)
    {
        const auto addKeyMapping = [this](Key key, KeyState KeyStates::* toKeyStates)
        {
            keyMappings.emplace(key, KeyMapping{ toKeyStates });
        };

        addKeyMapping(Key::LeftMouseButton, &KeyStates::leftMouseButton);
        addKeyMapping(Key::MiddleMouseButton, &KeyStates::middleMouseButton);
        addKeyMapping(Key::RightMouseButton, &KeyStates::rightMouseButton);
        addKeyMapping(Key::X1MouseButton, &KeyStates::x1MouseButton);
        addKeyMapping(Key::X2MouseButton, &KeyStates::x2MouseButton);

        addKeyMapping(Key::A, &KeyStates::a);
        addKeyMapping(Key::B, &KeyStates::b);
        addKeyMapping(Key::C, &KeyStates::c);
        addKeyMapping(Key::D, &KeyStates::d);
        addKeyMapping(Key::E, &KeyStates::e);
        addKeyMapping(Key::F, &KeyStates::f);
        addKeyMapping(Key::G, &KeyStates::g);
        addKeyMapping(Key::H, &KeyStates::h);
        addKeyMapping(Key::I, &KeyStates::i);
        addKeyMapping(Key::J, &KeyStates::j);
        addKeyMapping(Key::K, &KeyStates::k);
        addKeyMapping(Key::L, &KeyStates::l);
        addKeyMapping(Key::M, &KeyStates::m);
        addKeyMapping(Key::N, &KeyStates::n);
        addKeyMapping(Key::O, &KeyStates::o);
        addKeyMapping(Key::P, &KeyStates::p);
        addKeyMapping(Key::Q, &KeyStates::q);
        addKeyMapping(Key::R, &KeyStates::r);
        addKeyMapping(Key::S, &KeyStates::s);
        addKeyMapping(Key::T, &KeyStates::t);
        addKeyMapping(Key::U, &KeyStates::u);
        addKeyMapping(Key::V, &KeyStates::v);
        addKeyMapping(Key::W, &KeyStates::w);
        addKeyMapping(Key::X, &KeyStates::x);
        addKeyMapping(Key::Y, &KeyStates::y);
        addKeyMapping(Key::Z, &KeyStates::z);

        addKeyMapping(Key::Zero, &KeyStates::zero);
        addKeyMapping(Key::One, &KeyStates::one);
        addKeyMapping(Key::Two, &KeyStates::two);
        addKeyMapping(Key::Three, &KeyStates::three);
        addKeyMapping(Key::Four, &KeyStates::four);
        addKeyMapping(Key::Five, &KeyStates::five);
        addKeyMapping(Key::Six, &KeyStates::six);
        addKeyMapping(Key::Seven, &KeyStates::seven);
        addKeyMapping(Key::Eight, &KeyStates::eight);
        addKeyMapping(Key::Nine, &KeyStates::nine);

        addKeyMapping(Key::Left, &KeyStates::left);
        addKeyMapping(Key::Up, &KeyStates::up);
        addKeyMapping(Key::Right, &KeyStates::right);
        addKeyMapping(Key::Down, &KeyStates::down);

        addKeyMapping(Key::F1, &KeyStates::f1);
        addKeyMapping(Key::F2, &KeyStates::f2);
        addKeyMapping(Key::F3, &KeyStates::f3);
        addKeyMapping(Key::F4, &KeyStates::f4);
        addKeyMapping(Key::F5, &KeyStates::f5);
        addKeyMapping(Key::F6, &KeyStates::f6);
        addKeyMapping(Key::F7, &KeyStates::f7);
        addKeyMapping(Key::F8, &KeyStates::f8);
        addKeyMapping(Key::F9, &KeyStates::f9);
        addKeyMapping(Key::F10, &KeyStates::f10);
        addKeyMapping(Key::F11, &KeyStates::f11);
        addKeyMapping(Key::F12, &KeyStates::f12);
        addKeyMapping(Key::F13, &KeyStates::f13);
        addKeyMapping(Key::F14, &KeyStates::f14);
        addKeyMapping(Key::F15, &KeyStates::f15);
        addKeyMapping(Key::F16, &KeyStates::f16);
        addKeyMapping(Key::F17, &KeyStates::f17);
        addKeyMapping(Key::F18, &KeyStates::f18);
        addKeyMapping(Key::F19, &KeyStates::f19);
        addKeyMapping(Key::F20, &KeyStates::f20);
        addKeyMapping(Key::F21, &KeyStates::f21);
        addKeyMapping(Key::F22, &KeyStates::f22);
        addKeyMapping(Key::F23, &KeyStates::f23);
        addKeyMapping(Key::F24, &KeyStates::f24);

        addKeyMapping(Key::LeftAlt, &KeyStates::leftAlt);
        addKeyMapping(Key::LeftControl, &KeyStates::leftControl);
        addKeyMapping(Key::LeftShift, &KeyStates::leftShift);
        addKeyMapping(Key::LeftBracket, &KeyStates::leftBracket);
        addKeyMapping(Key::LeftGui, &KeyStates::leftGui);

        addKeyMapping(Key::RightAlt, &KeyStates::rightAlt);
        addKeyMapping(Key::RightControl, &KeyStates::rightControl);
        addKeyMapping(Key::RightShift, &KeyStates::rightShift);
        addKeyMapping(Key::RightBracket, &KeyStates::rightBracket);
        addKeyMapping(Key::RightGui, &KeyStates::rightGui);

        addKeyMapping(Key::PageDown, &KeyStates::pageDown);
        addKeyMapping(Key::PageUp, &KeyStates::pageUp);

        addKeyMapping(Key::Keypad0, &KeyStates::keypad0);
        addKeyMapping(Key::Keypad00, &KeyStates::keypad00);
        addKeyMapping(Key::Keypad000, &KeyStates::keypad000);
        addKeyMapping(Key::Keypad1, &KeyStates::keypad1);
        addKeyMapping(Key::Keypad2, &KeyStates::keypad2);
        addKeyMapping(Key::Keypad3, &KeyStates::keypad3);
        addKeyMapping(Key::Keypad4, &KeyStates::keypad4);
        addKeyMapping(Key::Keypad5, &KeyStates::keypad5);
        addKeyMapping(Key::Keypad6, &KeyStates::keypad6);
        addKeyMapping(Key::Keypad7, &KeyStates::keypad7);
        addKeyMapping(Key::Keypad8, &KeyStates::keypad8);
        addKeyMapping(Key::Keypad9, &KeyStates::keypad9);
        addKeyMapping(Key::KeypadA, &KeyStates::keypadA);
        addKeyMapping(Key::KeypadB, &KeyStates::keypadB);
        addKeyMapping(Key::KeypadC, &KeyStates::keypadC);
        addKeyMapping(Key::KeypadD, &KeyStates::keypadD);
        addKeyMapping(Key::KeypadE, &KeyStates::keypadE);
        addKeyMapping(Key::KeypadF, &KeyStates::keypadF);
        addKeyMapping(Key::KeypadAmpersand, &KeyStates::keypadAmpersand);
        addKeyMapping(Key::KeypadAt, &KeyStates::keypadAt);
        addKeyMapping(Key::KeypadBackspace, &KeyStates::keypadBackspace);
        addKeyMapping(Key::KeypadBinary, &KeyStates::keypadBinary);
        addKeyMapping(Key::KeypadClear, &KeyStates::keypadClear);
        addKeyMapping(Key::KeypadClearEntry, &KeyStates::keypadClearEntry);
        addKeyMapping(Key::KeypadColon, &KeyStates::keypadColon);
        addKeyMapping(Key::KeypadComma, &KeyStates::keypadComma);
        addKeyMapping(Key::KeypadAmpersandAmpersand, &KeyStates::keypadAmpersandAmpersand);
        addKeyMapping(Key::KeypadBarBar, &KeyStates::keypadBarBar);
        addKeyMapping(Key::KeypadDecimal, &KeyStates::keypadDecimal);
        addKeyMapping(Key::KeypadForwardSlash, &KeyStates::keypadForwardSlash);
        addKeyMapping(Key::KeypadEnter, &KeyStates::keypadEnter);
        addKeyMapping(Key::KeypadEquals, &KeyStates::keypadEquals);
        addKeyMapping(Key::KeypadEqualsAS400, &KeyStates::keypadEqualsAS400);
        addKeyMapping(Key::KeypadExclamation, &KeyStates::keypadExclamation);
        addKeyMapping(Key::KeypadGreater, &KeyStates::keypadGreater);
        addKeyMapping(Key::KeypadLess, &KeyStates::keypadLess);
        addKeyMapping(Key::KeypadPound, &KeyStates::keypadPound);
        addKeyMapping(Key::KeypadHexadecimal, &KeyStates::keypadHexadecimal);
        addKeyMapping(Key::KeypadLeftBrace, &KeyStates::keypadLeftBrace);
        addKeyMapping(Key::KeypadLeftParenthesis, &KeyStates::keypadLeftParenthesis);
        addKeyMapping(Key::KeypadMemAdd, &KeyStates::keypadMemAdd);
        addKeyMapping(Key::KeypadMemClear, &KeyStates::keypadMemClear);
        addKeyMapping(Key::KeypadMemDivide, &KeyStates::keypadMemDivide);
        addKeyMapping(Key::KeypadMemMultiply, &KeyStates::keypadMemMultiply);
        addKeyMapping(Key::KeypadMemRecall, &KeyStates::keypadMemRecall);
        addKeyMapping(Key::KeypadMemStore, &KeyStates::keypadMemStore);
        addKeyMapping(Key::KeypadMemSubtract, &KeyStates::keypadMemSubtract);
        addKeyMapping(Key::KeypadHyphen, &KeyStates::keypadHyphen);
        addKeyMapping(Key::KeypadAsterisk, &KeyStates::keypadAsterisk);
        addKeyMapping(Key::KeypadOctal, &KeyStates::keypadOctal);
        addKeyMapping(Key::KeypadPercent, &KeyStates::keypadPercent);
        addKeyMapping(Key::KeypadPeriod, &KeyStates::keypadPeriod);
        addKeyMapping(Key::KeypadPlus, &KeyStates::keypadPlus);
        addKeyMapping(Key::KeypadPlusMinus, &KeyStates::keypadPlusMinus);
        addKeyMapping(Key::KeypadPower, &KeyStates::keypadPower);
        addKeyMapping(Key::KeypadRightBrace, &KeyStates::keypadRightBrace);
        addKeyMapping(Key::KeypadRightParenthesis, &KeyStates::keypadRightParenthesis);
        addKeyMapping(Key::KeypadSpace, &KeyStates::keypadSpace);
        addKeyMapping(Key::KeypadTab, &KeyStates::keypadTab);
        addKeyMapping(Key::KeypadBar, &KeyStates::keypadBar);
        addKeyMapping(Key::KeypadXor, &KeyStates::keypadXor);

        addKeyMapping(Key::BrightnessUp, &KeyStates::brightnessUp);
        addKeyMapping(Key::BrightnessDown, &KeyStates::brightnessDown);

        addKeyMapping(Key::VolumeUp, &KeyStates::volumeUp);
        addKeyMapping(Key::VolumeDown, &KeyStates::volumeDown);

        addKeyMapping(Key::AudioMute, &KeyStates::audioMute);
        addKeyMapping(Key::AudioNext, &KeyStates::audioNext);
        addKeyMapping(Key::AudioPlay, &KeyStates::audioPlay);
        addKeyMapping(Key::AudioPrevious, &KeyStates::audioPrevious);
        addKeyMapping(Key::AudioStop, &KeyStates::audioStop);

        addKeyMapping(Key::AcBack, &KeyStates::acBack);
        addKeyMapping(Key::AcBookmarks, &KeyStates::acBookmarks);
        addKeyMapping(Key::AcForward, &KeyStates::acForward);
        addKeyMapping(Key::AcHome, &KeyStates::acHome);
        addKeyMapping(Key::AcRefresh, &KeyStates::acRefresh);
        addKeyMapping(Key::AcSearch, &KeyStates::acSearch);
        addKeyMapping(Key::AcStop, &KeyStates::acStop);

        addKeyMapping(Key::IlluminationDown, &KeyStates::illuminationDown);
        addKeyMapping(Key::IlluminationUp, &KeyStates::illuminationUp);
        addKeyMapping(Key::IlluminationToggle, &KeyStates::illuminationToggle);

        addKeyMapping(Key::Backslash, &KeyStates::backslash);
        addKeyMapping(Key::Backspace, &KeyStates::backspace);
        addKeyMapping(Key::Comma, &KeyStates::comma);
        addKeyMapping(Key::Quote, &KeyStates::quote);
        addKeyMapping(Key::Again, &KeyStates::again);
        addKeyMapping(Key::AltErase, &KeyStates::altErase);
        addKeyMapping(Key::Application, &KeyStates::application);
        addKeyMapping(Key::Calculator, &KeyStates::calculator);
        addKeyMapping(Key::Cancel, &KeyStates::cancel);
        addKeyMapping(Key::CapsLock, &KeyStates::capsLock);
        addKeyMapping(Key::Clear, &KeyStates::clear);
        addKeyMapping(Key::ClearAgain, &KeyStates::clearAgain);
        addKeyMapping(Key::Computer, &KeyStates::computer);
        addKeyMapping(Key::Copy, &KeyStates::copy);
        addKeyMapping(Key::CrSel, &KeyStates::crSel);
        addKeyMapping(Key::CurrencySubUnit, &KeyStates::currencySubUnit);
        addKeyMapping(Key::CurrencyUnit, &KeyStates::currencyUnit);
        addKeyMapping(Key::Cut, &KeyStates::cut);
        addKeyMapping(Key::DecimalSeparator, &KeyStates::decimalSeparator);
        addKeyMapping(Key::Del, &KeyStates::del);
        addKeyMapping(Key::DisplaySwitch, &KeyStates::displaySwitch);
        addKeyMapping(Key::Eject, &KeyStates::eject);
        addKeyMapping(Key::End, &KeyStates::end);
        addKeyMapping(Key::Equals, &KeyStates::equals);
        addKeyMapping(Key::Escape, &KeyStates::escape);
        addKeyMapping(Key::Execute, &KeyStates::execute);
        addKeyMapping(Key::ExSel, &KeyStates::exSel);
        addKeyMapping(Key::Find, &KeyStates::find);
        addKeyMapping(Key::Grave, &KeyStates::grave);
        addKeyMapping(Key::Help, &KeyStates::help);
        addKeyMapping(Key::Home, &KeyStates::home);
        addKeyMapping(Key::Insert, &KeyStates::insert);
        addKeyMapping(Key::Mail, &KeyStates::mail);
        addKeyMapping(Key::MediaSelect, &KeyStates::mediaSelect);
        addKeyMapping(Key::Menu, &KeyStates::menu);
        addKeyMapping(Key::Hyphen, &KeyStates::hyphen);
        addKeyMapping(Key::ModeSwitch, &KeyStates::modeSwitch);
        addKeyMapping(Key::Mute, &KeyStates::mute);
        addKeyMapping(Key::Numlock, &KeyStates::numlock);
        addKeyMapping(Key::Oper, &KeyStates::oper);
        addKeyMapping(Key::Out, &KeyStates::out);
        addKeyMapping(Key::Paste, &KeyStates::paste);
        addKeyMapping(Key::Pause, &KeyStates::pause);
        addKeyMapping(Key::Period, &KeyStates::period);
        addKeyMapping(Key::Power, &KeyStates::power);
        addKeyMapping(Key::PrintScreen, &KeyStates::printScreen);
        addKeyMapping(Key::Prior, &KeyStates::prior);
        addKeyMapping(Key::Enter, &KeyStates::enter);
        addKeyMapping(Key::ScrollLock, &KeyStates::scrollLock);
        addKeyMapping(Key::Select, &KeyStates::select);
        addKeyMapping(Key::Semicolon, &KeyStates::semicolon);
        addKeyMapping(Key::Separator, &KeyStates::separator);
        addKeyMapping(Key::ForwardSlash, &KeyStates::forwardSlash);
        addKeyMapping(Key::Sleep, &KeyStates::sleep);
        addKeyMapping(Key::Space, &KeyStates::space);
        addKeyMapping(Key::Stop, &KeyStates::stop);
        addKeyMapping(Key::SysReq, &KeyStates::sysReq);
        addKeyMapping(Key::Tab, &KeyStates::tab);
        addKeyMapping(Key::ThousandsSeparator, &KeyStates::thousandsSeparator);
        addKeyMapping(Key::Undo, &KeyStates::undo);
        addKeyMapping(Key::WorldWideWeb, &KeyStates::worldWideWeb);
    }

    void Curator::Handle(const BindAction& command)
    {
        auto action = MutablePointer().Of(command.action);
        if (!action)
            return;

        action->mappedKeys = command.keys;
    }

    void Curator::Handle(const Work&)
    {
        const auto inputState = manager->ReadState();

        keyStates.current = inputState.keyStates;
        mousePosition.current = inputState.mousePosition;

        const auto actions = Owner().Batch<Asset::Action>();
        for(auto action = actions.begin(); action != actions.end(); ++action)
        {
            const auto actionIndex = Arca::Index<Asset::Action>{ action.ID(), Owner() };

            const auto isActive = IsActive(action->mappedKeys, keyStates.current);
            if (isActive)
                Owner().Raise(ActionActive{ actionIndex });

            const auto actionDelta = ActionDelta(action->mappedKeys, keyStates);

            if (actionDelta == KeyState::Down)
            {
                Owner().Do(Logging::Log("Down"));
                Owner().Raise(ActionPressed{ actionIndex });
            }
            else if (actionDelta == KeyState::Up)
            {
                Owner().Do(Logging::Log("Up"));
                Owner().Raise(ActionDepressed{ actionIndex });
            }
        }

        if (mousePosition.current != mousePosition.previous)
            Owner().Raise(MouseMoved{ mousePosition.previous, mousePosition.current });

        for (auto& text : inputState.enteredText)
            Owner().Raise(TextEntered{ text });

        keyStates.previous = keyStates.current;
        mousePosition.previous = mousePosition.current;
    }

    bool Curator::IsActive(std::set<Key> keys, KeyStates currentKeyStates) const
    {
        for(auto& key : keys)
        {
            const auto& keyMapping = RequiredKeyMapping(key);
            if (currentKeyStates.*keyMapping.toKeyStates == KeyState::Up)
                return false;
        }

        return true;
    }

    std::optional<KeyState> Curator::ActionDelta(std::set<Key> keys, Delta<KeyStates> allKeyStates) const
    {
        if (keys.empty())
            return {};

        const auto retrieveKeyDelta = [this, allKeyStates](Key key) -> std::optional<KeyState>
        {
            const auto& keyMapping = RequiredKeyMapping(key);
            const auto previousKeyState = allKeyStates.previous.*keyMapping.toKeyStates;
            const auto currentKeyState = allKeyStates.current.*keyMapping.toKeyStates;

            if (currentKeyState == previousKeyState)
                return {};
            else
                return currentKeyState;
        };

        const auto actionDelta = retrieveKeyDelta(*keys.begin());
        for (auto key = std::next(keys.begin()); key != keys.end(); ++key)
            if (retrieveKeyDelta(*key) != actionDelta)
                return {};

        return actionDelta;
    }

    auto Curator::RequiredKeyMapping(Key key) const -> const KeyMapping&
    {
        const auto found = keyMappings.find(key);
        if (found == keyMappings.end())
            throw std::runtime_error("Required key mapping not found.");
        else
            return found->second;
    }
}
