#pragma once

#include "IniID.h"

#include "InputDefines.h"
#include "Volume.h"
#include <AGUI/Resolution.h>
#include "StringUtility.h"

#include "Optional.h"

namespace Atmos
{
    namespace Ini
    {
        template<class T>
        String ToIniString(const T &arg)
        {
            return ::Atmos::ToString(arg);
        }
        String ToIniString(const ::Agui::Resolution::Size &arg);
        String ToIniString(const Input::KeyID &arg);

        template<class T>
        Optional<T> FromIniString(const String &arg)
        {
            return ::Atmos::FromString<T>(arg);
        }
        template<>
        Optional<::Agui::Resolution::Size> FromIniString(const String &arg);
        template<>
        Optional<Input::KeyID> FromIniString(const String &arg);

        template<ID>
        struct Traits;

        template<> struct Traits<ID::WINDOWED> { typedef bool T; };
        template<> struct Traits<ID::RESOLUTION> { typedef ::Agui::Resolution::Size T; };
        template<> struct Traits<ID::FPS> { typedef unsigned int T; };
        template<> struct Traits<ID::VSYNC> { typedef bool T; };

        template<> struct Traits<ID::MASTER_SOUND> { typedef Volume T; };

        template<> struct Traits<ID::MOVE_LEFT> { typedef Atmos::Input::KeyID T; };
        template<> struct Traits<ID::MOVE_UP> { typedef Atmos::Input::KeyID T; };
        template<> struct Traits<ID::MOVE_RIGHT> { typedef Atmos::Input::KeyID T; };
        template<> struct Traits<ID::MOVE_DOWN> { typedef Atmos::Input::KeyID T; };
        template<> struct Traits<ID::USE> { typedef Atmos::Input::KeyID T; };
        template<> struct Traits<ID::MENU_LEFT> { typedef Atmos::Input::KeyID T; };
        template<> struct Traits<ID::MENU_UP> { typedef Atmos::Input::KeyID T; };
        template<> struct Traits<ID::MENU_RIGHT> { typedef Atmos::Input::KeyID T; };
        template<> struct Traits<ID::MENU_DOWN> { typedef Atmos::Input::KeyID T; };
        template<> struct Traits<ID::SELECT_CHARACTER_LEFT> { typedef Atmos::Input::KeyID T; };
        template<> struct Traits<ID::SELECT_CHARACTER_RIGHT> { typedef Atmos::Input::KeyID T; };
        template<> struct Traits<ID::INVENTORY> { typedef Atmos::Input::KeyID T; };
        template<> struct Traits<ID::END_TURN> { typedef Atmos::Input::KeyID T; };
        template<> struct Traits<ID::ATTACK> { typedef Atmos::Input::KeyID T; };
        template<> struct Traits<ID::SPELLS> { typedef Atmos::Input::KeyID T; };
        template<> struct Traits<ID::CANCEL> { typedef Atmos::Input::KeyID T; };
        template<> struct Traits<ID::STATS> { typedef Atmos::Input::KeyID T; };
    }
}