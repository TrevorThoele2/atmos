#pragma once

#include <sstream>
#include <type_traits>

#include "String.h"
#include "TimePoint.h"
#include "Point3D.h"
#include "Size2D.h"
#include "Variant.h"

#include <Chroma/TypeIdentity.h>
#include <Arca/SlimHandle.h>

namespace Atmos
{
    namespace Spatial::Grid
    {
        struct Point;
    }

    void SpliceString(String& in, const String& check, const String& replace);

    String Trim(const String& trim);
    bool IsAllWhitespace(const String& check);
    bool StartsWith(const String& check, const String& startsWith);

    template<class T>
    void SpliceString(String& in, const String& check, const T replace)
    {
        SpliceString(in, check, String(replace));
    }

    template<class T, std::enable_if_t<!std::is_enum_v<T>, int> = 0>
    T FromString(const String& arg);

    template<class T, std::enable_if_t<std::is_enum_v<T>, int> = 0>
    T FromString(const String& arg);

    namespace detail
    {
        template<class T>
        T FromStringImpl(const String& arg, Chroma::TypeIdentity<T>)
        {
            std::istringstream stream(arg);

            T toReturn;
            stream >> toReturn;
            return toReturn;
        }

        String FromStringImpl(const String &arg, Chroma::TypeIdentity<String>);

        template<class Duration>
        Time::Point<Duration> FromStringImpl(const String& arg, Chroma::TypeIdentity<Time::Point<Duration>>)
        {
            return Time::Point<Duration>(FromString<Duration>(arg));
        }

        template<class Ratio>
        Time::Duration<Ratio> FromStringImpl(const String& arg, Chroma::TypeIdentity<Time::Duration<Ratio>>)
        {
            return Time::Duration<Ratio>(FromString<long long>(arg));
        }
    }

    template<class T, std::enable_if_t<!std::is_enum_v<T>, int>>
    T FromString(const String& arg)
    {
        return detail::FromStringImpl(arg, Chroma::TypeIdentity<T>{});
    }

    template<class T, std::enable_if_t<std::is_enum_v<T>, int>>
    T FromString(const String& arg)
    {
        return static_cast<T>(FromString<std::underlying_type<T>::type>(arg));
    }

    template<> char FromString(const String& arg);
    template<> signed char FromString(const String& arg);
    template<> unsigned char FromString(const String& arg);
    template<> short FromString(const String& arg);
    template<> unsigned short FromString(const String& arg);
    template<> bool FromString(const String& arg);

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
    String ToString(const String& arg);
    String ToString(bool arg);
    template<class Duration>
    String ToString(Time::Point<Duration> timeValue)
    {
        return ToString(timeValue.time_since_epoch().count());
    }
    template<class Ratio>
    String ToString(Time::Duration<Ratio> duration)
    {
        return ToString(duration.count());
    }
    String ToString(const Spatial::Grid::Point& position);
    String ToString(const Spatial::Point3D& position);
    String ToString(const Spatial::Size2D& size);
    String ToString(const Arca::SlimHandle& handle);
    String ToString(const Variant& arg);
}