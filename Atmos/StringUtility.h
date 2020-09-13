#pragma once

#include <sstream>
#include <type_traits>

#include "String.h"
#include "TimeValue.h"
#include "Point3D.h"
#include "Size2D.h"

#include <Chroma/TypeIdentity.h>

namespace Atmos
{
    namespace Spatial::Grid
    {
        struct Point;
    }

    void SpliceString(String &in, const String &check, const String &replace);

    String Trim(const String& trim);
    bool IsAllWhitespace(const String& check);
    bool StartsWith(const String& check, const String& startsWith);

    template<class T>
    void SpliceString(String &in, const String &check, const T replace)
    {
        SpliceString(in, check, String(replace));
    }

    template<class T, typename ::std::enable_if<!::std::is_enum<T>::value, int>::type = 0>
    T FromString(const String& arg);

    template<class T, typename ::std::enable_if<::std::is_enum<T>::value, int>::type = 0>
    T FromString(const String& arg);

    namespace detail
    {
        template<class T>
        T FromStringImpl(const String &arg, const ::Chroma::TypeIdentity<T> &t)
        {
            ::std::istringstream stream(arg);

            T toReturn;
            stream >> toReturn;
            return toReturn;
        }

        String FromStringImpl(const String &arg, const ::Chroma::TypeIdentity<String>& t);

        template<class Duration>
        Time::Value<Duration> FromStringImpl(const String& arg, const ::Chroma::TypeIdentity<Time::Value<Duration>>& t)
        {
            return Time::Value<Duration>(FromString<Duration>(arg));
        }

        template<class Rep, class Ratio>
        Time::Duration<Rep, Ratio> FromStringImpl(const String& arg, const ::Chroma::TypeIdentity<Time::Duration<Rep, Ratio>>& t)
        {
            return Time::Duration<Rep, Ratio>(FromString<long long>(arg));
        }
    }

    template<class T, typename ::std::enable_if<!::std::is_enum<T>::value, int>::type>
    T FromString(const String &arg)
    {
        return detail::FromStringImpl(arg, ::Chroma::TypeIdentity<T>{});
    }

    template<class T, typename ::std::enable_if<::std::is_enum<T>::value, int>::type>
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
    template<class Duration>
    String ToString(Time::Value<Duration> timeValue)
    {
        return ToString(timeValue.time_since_epoch().count());
    }
    template<class Rep, class Ratio>
    String ToString(Time::Duration<Rep, Ratio> duration)
    {
        return ToString(duration.count());
    }
    String ToString(const Spatial::Grid::Point &position);
    String ToString(const Spatial::Point3D& position);
    String ToString(const Spatial::Size2D& size);
}