#pragma once

#include <sstream>
#include <type_traits>

#include "String.h"
#include "LargeInteger.h"
#include "FixedPoint.h"
#include "TimeValue.h"
#include "InputKeyID.h"
#include "InputMouseKeyID.h"
#include "InputActionID.h"

#include <Chroma/Type.h>

#include <AGUI/Resolution.h>

namespace Atmos
{
    String GetFileName(const String &filePath);
    String GetFileExtension(const String &filePath);
    String RemoveFileExtension(const String &string);
    String RemoveFileName(const String &string);
    String ReplaceFileExtension(const String &string, const String &extension);

    void SpliceString(String &in, const String &check, const String &replace);

    String Trim(const String& trim);
    bool IsAllWhitespace(const String& check);
    bool StartsWith(const String& check, const String& startsWith);

    template<class T>
    void SpliceString(String &in, const String &check, const T replace)
    {
        SpliceString(in, check, String(replace));
    }

    namespace detail
    {
        template<class T>
        T FromStringImpl(const String &arg, const ::Chroma::Type<T> &t)
        {
            ::std::istringstream stream(arg);

            T toReturn;
            stream >> toReturn;
            return toReturn;
        }

        String FromStringImpl(const String &arg, const ::Chroma::Type<String> &t);

        template<class T>
        FixedPoint<T> FromStringImpl(const String &arg, const ::Chroma::Type<FixedPoint<T>> &t)
        {
            FixedPoint<T> ret;
            ret.FromString(arg);
            return ret;
        }

        TimeValue FromStringImpl(const String &arg, const ::Chroma::Type<TimeValue> &t);
    }

    template<class T, typename ::std::enable_if<!::std::is_enum<T>::value, int>::type = 0>
    T FromString(const String &arg)
    {
        return detail::FromStringImpl(arg, ::Chroma::Type<T>{});
    }

    template<class T, typename ::std::enable_if<::std::is_enum<T>::value, int>::type = 0>
    T FromString(const String &arg)
    {
        return static_cast<T>(FromString<::std::underlying_type<T>::type>(arg));
    }

    template<> char FromString(const String &arg);
    template<> signed char FromString(const String &arg);
    template<> unsigned char FromString(const String &arg);
    template<> short FromString(const String &arg);
    template<> unsigned short FromString(const String &arg);
    template<> bool FromString(const String &arg);
    template<> LargeInteger FromString(const String &arg);
    template<> ::Agui::Resolution::Size FromString(const String &arg);

    template<class T>
    String ToString(T arg)
    {
        std::ostringstream s;
        s << arg;
        return String(s.str());
    }

    String ToString(char arg);
    String ToString(signed char arg);
    String ToString(unsigned char arg);
    String ToString(const String &arg);
    String ToString(bool arg);
    String ToString(LargeInteger arg);
    template<class T>
    String ToString(FixedPoint<T> arg)
    {
        return arg.ToString();
    }
    String ToString(TimeValue timeValue);
    class GridPosition;
    String ToString(const GridPosition &position);
    String ToString(const ::Agui::Resolution::Size &arg);
    String ToString(const Input::KeyID &arg);
    String ToString(const Input::MouseKeyID &arg);
    String ToString(const Input::ActionID &arg);
}