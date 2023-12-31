#pragma once

#include "Serialization.h"

namespace Atmos::Input
{
    enum class Key
    {
        LeftMouseButton,
        MiddleMouseButton,
        RightMouseButton,
        X1MouseButton,
        X2MouseButton,

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

        Zero,
        One,
        Two,
        Three,
        Four,
        Five,
        Six,
        Seven,
        Eight,
        Nine,

        Left,
        Up,
        Right,
        Down,

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

        LeftAlt,
        LeftControl,
        LeftShift,
        LeftBracket,
        LeftGui,

        RightAlt,
        RightControl,
        RightShift,
        RightBracket,
        RightGui,

        PageDown,
        PageUp,

        Keypad0,
        Keypad00,
        Keypad000,
        Keypad1,
        Keypad2,
        Keypad3,
        Keypad4,
        Keypad5,
        Keypad6,
        Keypad7,
        Keypad8,
        Keypad9,
        KeypadA,
        KeypadB,
        KeypadC,
        KeypadD,
        KeypadE,
        KeypadF,
        KeypadAmpersand,
        KeypadAt,
        KeypadBackspace,
        KeypadBinary,
        KeypadClear,
        KeypadClearEntry,
        KeypadColon,
        KeypadComma,
        KeypadAmpersandAmpersand,
        KeypadBarBar,
        KeypadDecimal,
        KeypadForwardSlash,
        KeypadEnter,
        KeypadEquals,
        KeypadEqualsAS400,
        KeypadExclamation,
        KeypadGreater,
        KeypadLess,
        KeypadPound,
        KeypadHexadecimal,
        KeypadLeftBrace,
        KeypadLeftParenthesis,
        KeypadMemAdd,
        KeypadMemClear,
        KeypadMemDivide,
        KeypadMemMultiply,
        KeypadMemRecall,
        KeypadMemStore,
        KeypadMemSubtract,
        KeypadHyphen,
        KeypadAsterisk,
        KeypadOctal,
        KeypadPercent,
        KeypadPeriod,
        KeypadPlus,
        KeypadPlusMinus,
        KeypadPower,
        KeypadRightBrace,
        KeypadRightParenthesis,
        KeypadSpace,
        KeypadTab,
        KeypadBar,
        KeypadXor,

        BrightnessUp,
        BrightnessDown,

        VolumeUp,
        VolumeDown,

        AudioMute,
        AudioNext,
        AudioPlay,
        AudioPrevious,
        AudioStop,

        AcBack,
        AcBookmarks,
        AcForward,
        AcHome,
        AcRefresh,
        AcSearch,
        AcStop,

        IlluminationDown,
        IlluminationUp,
        IlluminationToggle,

        Backslash,
        Backspace,
        Comma,
        Quote,
        Again,
        AltErase,
        Application,
        Calculator,
        Cancel,
        CapsLock,
        Clear,
        ClearAgain,
        Computer,
        Copy,
        CrSel,
        CurrencySubUnit,
        CurrencyUnit,
        Cut,
        DecimalSeparator,
        Del,
        DisplaySwitch,
        Eject,
        End,
        Equals,
        Escape,
        Execute,
        ExSel,
        Find,
        Grave,
        Help,
        Home,
        Insert,
        Mail,
        MediaSelect,
        Menu,
        Hyphen,
        ModeSwitch,
        Mute,
        Numlock,
        Oper,
        Out,
        Paste,
        Pause,
        Period,
        Power,
        PrintScreen,
        Prior,
        Enter,
        ScrollLock,
        Select,
        Semicolon,
        Separator,
        ForwardSlash,
        Sleep,
        Space,
        Stop,
        SysReq,
        Tab,
        ThousandsSeparator,
        Undo,
        WorldWideWeb
    };
}

namespace Inscription
{
    template<class Format>
    struct ScribeTraits<Atmos::Input::Key, Format>
    {
        using Category = EnumScribeCategory<Atmos::Input::Key>;
    };
}