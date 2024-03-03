#include "AngelScriptKey.h"

#include <angelscript.h>
#include "AngelScriptEnumRegistration.h"

namespace Atmos::Scripting::Angel
{
    void Registration<Input::Key>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        EnumRegistration<Input::Key>(ContainingNamespace(), Name())
            .Value("LeftMouseButton", Input::Key::LeftMouseButton)
            .Value("MiddleMouseButton", Input::Key::MiddleMouseButton)
            .Value("RightMouseButton", Input::Key::RightMouseButton)
            .Value("X1MouseButton", Input::Key::X1MouseButton)
            .Value("X2MouseButton", Input::Key::X2MouseButton)

            .Value("A", Input::Key::A)
            .Value("B", Input::Key::B)
            .Value("C", Input::Key::C)
            .Value("D", Input::Key::D)
            .Value("E", Input::Key::E)
            .Value("F", Input::Key::F)
            .Value("G", Input::Key::G)
            .Value("H", Input::Key::H)
            .Value("I", Input::Key::I)
            .Value("J", Input::Key::J)
            .Value("K", Input::Key::K)
            .Value("L", Input::Key::L)
            .Value("M", Input::Key::M)
            .Value("N", Input::Key::N)
            .Value("O", Input::Key::O)
            .Value("P", Input::Key::P)
            .Value("Q", Input::Key::Q)
            .Value("R", Input::Key::R)
            .Value("S", Input::Key::S)
            .Value("T", Input::Key::T)
            .Value("U", Input::Key::U)
            .Value("V", Input::Key::V)
            .Value("W", Input::Key::W)
            .Value("X", Input::Key::X)
            .Value("Y", Input::Key::Y)
            .Value("Z", Input::Key::Z)

            .Value("Zero", Input::Key::Zero)
            .Value("One", Input::Key::One)
            .Value("Two", Input::Key::Two)
            .Value("Three", Input::Key::Three)
            .Value("Four", Input::Key::Four)
            .Value("Five", Input::Key::Five)
            .Value("Six", Input::Key::Six)
            .Value("Seven", Input::Key::Seven)
            .Value("Eight", Input::Key::Eight)
            .Value("Nine", Input::Key::Nine)

            .Value("Left", Input::Key::Left)
            .Value("Up", Input::Key::Up)
            .Value("Right", Input::Key::Right)
            .Value("Down", Input::Key::Down)

            .Value("F1", Input::Key::F1)
            .Value("F2", Input::Key::F2)
            .Value("F3", Input::Key::F3)
            .Value("F4", Input::Key::F4)
            .Value("F5", Input::Key::F5)
            .Value("F6", Input::Key::F6)
            .Value("F7", Input::Key::F7)
            .Value("F8", Input::Key::F8)
            .Value("F9", Input::Key::F9)
            .Value("F10", Input::Key::F10)
            .Value("F11", Input::Key::F11)
            .Value("F12", Input::Key::F12)
            .Value("F13", Input::Key::F13)
            .Value("F14", Input::Key::F14)
            .Value("F15", Input::Key::F15)
            .Value("F16", Input::Key::F16)
            .Value("F17", Input::Key::F17)
            .Value("F18", Input::Key::F18)
            .Value("F19", Input::Key::F19)
            .Value("F20", Input::Key::F20)
            .Value("F21", Input::Key::F21)
            .Value("F22", Input::Key::F22)
            .Value("F23", Input::Key::F23)
            .Value("F24", Input::Key::F24)

            .Value("LeftAlt", Input::Key::LeftAlt)
            .Value("LeftControl", Input::Key::LeftControl)
            .Value("LeftShift", Input::Key::LeftShift)
            .Value("LeftBracket", Input::Key::LeftBracket)
            .Value("LeftGui", Input::Key::LeftGui)

            .Value("RightAlt", Input::Key::RightAlt)
            .Value("RightControl", Input::Key::RightControl)
            .Value("RightShift", Input::Key::RightShift)
            .Value("RightBracket", Input::Key::RightBracket)
            .Value("RightGui", Input::Key::RightGui)

            .Value("PageDown", Input::Key::PageDown)
            .Value("PageUp", Input::Key::PageUp)

            .Value("Keypad0", Input::Key::Keypad0)
            .Value("Keypad00", Input::Key::Keypad00)
            .Value("Keypad000", Input::Key::Keypad000)
            .Value("Keypad1", Input::Key::Keypad1)
            .Value("Keypad2", Input::Key::Keypad2)
            .Value("Keypad3", Input::Key::Keypad3)
            .Value("Keypad4", Input::Key::Keypad4)
            .Value("Keypad5", Input::Key::Keypad5)
            .Value("Keypad6", Input::Key::Keypad6)
            .Value("Keypad7", Input::Key::Keypad7)
            .Value("Keypad8", Input::Key::Keypad8)
            .Value("Keypad9", Input::Key::Keypad9)
            .Value("KeypadA", Input::Key::KeypadA)
            .Value("KeypadB", Input::Key::KeypadB)
            .Value("KeypadC", Input::Key::KeypadC)
            .Value("KeypadD", Input::Key::KeypadD)
            .Value("KeypadE", Input::Key::KeypadE)
            .Value("KeypadF", Input::Key::KeypadF)
            .Value("KeypadAmpersand", Input::Key::KeypadAmpersand)
            .Value("KeypadAt", Input::Key::KeypadAt)
            .Value("KeypadBackspace", Input::Key::KeypadBackspace)
            .Value("KeypadBinary", Input::Key::KeypadBinary)
            .Value("KeypadClear", Input::Key::KeypadClear)
            .Value("KeypadClearEntry", Input::Key::KeypadClearEntry)
            .Value("KeypadColon", Input::Key::KeypadColon)
            .Value("KeypadComma", Input::Key::KeypadComma)
            .Value("KeypadAmpersandAmpersand", Input::Key::KeypadAmpersandAmpersand)
            .Value("KeypadBarBar", Input::Key::KeypadBarBar)
            .Value("KeypadDecimal", Input::Key::KeypadDecimal)
            .Value("KeypadForwardSlash", Input::Key::KeypadForwardSlash)
            .Value("KeypadEnter", Input::Key::KeypadEnter)
            .Value("KeypadEquals", Input::Key::KeypadEquals)
            .Value("KeypadEqualsAS400", Input::Key::KeypadEqualsAS400)
            .Value("KeypadExclamation", Input::Key::KeypadExclamation)
            .Value("KeypadGreater", Input::Key::KeypadGreater)
            .Value("KeypadLess", Input::Key::KeypadLess)
            .Value("KeypadPound", Input::Key::KeypadPound)
            .Value("KeypadHexadecimal", Input::Key::KeypadHexadecimal)
            .Value("KeypadLeftBrace", Input::Key::KeypadLeftBrace)
            .Value("KeypadLeftParenthesis", Input::Key::KeypadLeftParenthesis)
            .Value("KeypadMemAdd", Input::Key::KeypadMemAdd)
            .Value("KeypadMemClear", Input::Key::KeypadMemClear)
            .Value("KeypadMemDivide", Input::Key::KeypadMemDivide)
            .Value("KeypadMemMultiply", Input::Key::KeypadMemMultiply)
            .Value("KeypadMemRecall", Input::Key::KeypadMemRecall)
            .Value("KeypadMemStore", Input::Key::KeypadMemStore)
            .Value("KeypadMemSubtract", Input::Key::KeypadMemSubtract)
            .Value("KeypadHyphen", Input::Key::KeypadHyphen)
            .Value("KeypadAsterisk", Input::Key::KeypadAsterisk)
            .Value("KeypadOctal", Input::Key::KeypadOctal)
            .Value("KeypadPercent", Input::Key::KeypadPercent)
            .Value("KeypadPeriod", Input::Key::KeypadPeriod)
            .Value("KeypadPlus", Input::Key::KeypadPlus)
            .Value("KeypadPlusMinus", Input::Key::KeypadPlusMinus)
            .Value("KeypadPower", Input::Key::KeypadPower)
            .Value("KeypadRightBrace", Input::Key::KeypadRightBrace)
            .Value("KeypadRightParenthesis", Input::Key::KeypadRightParenthesis)
            .Value("KeypadSpace", Input::Key::KeypadSpace)
            .Value("KeypadTab", Input::Key::KeypadTab)
            .Value("KeypadBar", Input::Key::KeypadBar)
            .Value("KeypadXor", Input::Key::KeypadXor)

            .Value("BrightnessUp", Input::Key::BrightnessUp)
            .Value("BrightnessDown", Input::Key::BrightnessDown)

            .Value("VolumeUp", Input::Key::VolumeUp)
            .Value("VolumeDown", Input::Key::VolumeDown)

            .Value("AudioMute", Input::Key::AudioMute)
            .Value("AudioNext", Input::Key::AudioNext)
            .Value("AudioPlay", Input::Key::AudioPlay)
            .Value("AudioPrevious", Input::Key::AudioPrevious)
            .Value("AudioStop", Input::Key::AudioStop)

            .Value("AcBack", Input::Key::AcBack)
            .Value("AcBookmarks", Input::Key::AcBookmarks)
            .Value("AcForward", Input::Key::AcForward)
            .Value("AcHome", Input::Key::AcHome)
            .Value("AcRefresh", Input::Key::AcRefresh)
            .Value("AcSearch", Input::Key::AcSearch)
            .Value("AcStop", Input::Key::AcStop)

            .Value("IlluminationDown", Input::Key::IlluminationDown)
            .Value("IlluminationUp", Input::Key::IlluminationUp)
            .Value("IlluminationToggle", Input::Key::IlluminationToggle)

            .Value("Backslash", Input::Key::Backslash)
            .Value("Backspace", Input::Key::Backspace)
            .Value("Comma", Input::Key::Comma)
            .Value("Quote", Input::Key::Quote)
            .Value("Again", Input::Key::Again)
            .Value("AltErase", Input::Key::AltErase)
            .Value("Application", Input::Key::Application)
            .Value("Calculator", Input::Key::Calculator)
            .Value("Cancel", Input::Key::Cancel)
            .Value("CapsLock", Input::Key::CapsLock)
            .Value("Clear", Input::Key::Clear)
            .Value("ClearAgain", Input::Key::ClearAgain)
            .Value("Computer", Input::Key::Computer)
            .Value("Copy", Input::Key::Copy)
            .Value("CrSel", Input::Key::CrSel)
            .Value("CurrencySubUnit", Input::Key::CurrencySubUnit)
            .Value("CurrencyUnit", Input::Key::CurrencyUnit)
            .Value("Cut", Input::Key::Cut)
            .Value("DecimalSeparator", Input::Key::DecimalSeparator)
            .Value("Del", Input::Key::Del)
            .Value("DisplaySwitch", Input::Key::DisplaySwitch)
            .Value("Eject", Input::Key::Eject)
            .Value("End", Input::Key::End)
            .Value("Equals", Input::Key::Equals)
            .Value("Escape", Input::Key::Escape)
            .Value("Execute", Input::Key::Execute)
            .Value("ExSel", Input::Key::ExSel)
            .Value("Find", Input::Key::Find)
            .Value("Grave", Input::Key::Grave)
            .Value("Help", Input::Key::Help)
            .Value("Home", Input::Key::Home)
            .Value("Insert", Input::Key::Insert)
            .Value("Mail", Input::Key::Mail)
            .Value("MediaSelect", Input::Key::MediaSelect)
            .Value("Menu", Input::Key::Menu)
            .Value("Hyphen", Input::Key::Hyphen)
            .Value("ModeSwitch", Input::Key::ModeSwitch)
            .Value("Mute", Input::Key::Mute)
            .Value("Numlock", Input::Key::Numlock)
            .Value("Oper", Input::Key::Oper)
            .Value("Out", Input::Key::Out)
            .Value("Paste", Input::Key::Paste)
            .Value("Pause", Input::Key::Pause)
            .Value("Period", Input::Key::Period)
            .Value("Power", Input::Key::Power)
            .Value("PrintScreen", Input::Key::PrintScreen)
            .Value("Prior", Input::Key::Prior)
            .Value("Enter", Input::Key::Enter)
            .Value("ScrollLock", Input::Key::ScrollLock)
            .Value("Select", Input::Key::Select)
            .Value("Semicolon", Input::Key::Semicolon)
            .Value("Separator", Input::Key::Separator)
            .Value("ForwardSlash", Input::Key::ForwardSlash)
            .Value("Sleep", Input::Key::Sleep)
            .Value("Space", Input::Key::Space)
            .Value("Stop", Input::Key::Stop)
            .Value("SysReq", Input::Key::SysReq)
            .Value("Tab", Input::Key::Tab)
            .Value("ThousandsSeparator", Input::Key::ThousandsSeparator)
            .Value("Undo", Input::Key::Undo)
            .Value("WorldWideWeb", Input::Key::WorldWideWeb)
            .Actualize(engine, documentationManager);
    }
}