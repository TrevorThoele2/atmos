
#include "StringUtility.h"

namespace Atmos
{
    String GetFileName(const String &filePath)
    {
        String newString = filePath;
        if (newString.find("\\") != newString.npos)
        {
            // Slice the file path to just its file name
            auto pos = newString.find_last_of("\\") + 1;
            newString = newString.substr(pos);
        }

        return newString;
    }

    String GetFileExtension(const String &filePath)
    {
        String newString = filePath;
        if (newString.find(".") != newString.npos)
        {
            // Slice the file path to just its file name
            auto pos = newString.find_last_of(".") + 1;
            return newString.substr(pos);
        }

        return newString;
    }

    String RemoveFileExtension(const String &string)
    {
        String newString = string;
        if (newString.find(".") != newString.npos)
        {
            // Slice the file extension off
            auto pos = newString.find_last_of(".");
            newString.erase(pos);
        }

        return newString;
    }

    String RemoveFileName(const String &string)
    {
        String newString = string;
        if (newString.find(".") != newString.npos)
        {
            // Slice the file extension off
            auto pos = newString.find_last_of("\\");
            newString.erase(pos);
        }

        return newString;
    }

    String ReplaceFileExtension(const String &string, const String &extension)
    {
        String newString = string;
        if (newString.find(".") != newString.npos)
        {
            // Slice the file extension off
            auto pos = newString.find_last_of(".");
            newString.replace(pos + 1, String::npos, extension);
        }
        else
            newString.append('.' + extension);

        return newString;
    }

    void SpliceString(String &in, const String &check, const String &replace)
    {
        auto pos = in.find(check);

        while (pos != in.npos)
        {
            in.erase(pos, check.size());
            in.insert(pos, replace);

            pos = in.find(check);
        }
    }

    namespace Detail
    {
        String FromStringImpl(const String &arg, const ::function::Type<String> &t)
        {
            return arg;
        }

        TimeValue FromStringImpl(const String &arg, const ::function::Type<TimeValue> &t)
        {
            return FromString<TimeValue::ValueT>(arg);
        }

        GameTimeValue FromStringImpl(const String &arg, const ::function::Type<GameTimeValue> &t)
        {
            return FromString<GameTimeValue::ValueT>(arg);
        }
    }

    template<class T>
    T FromStringCommon(const String &arg)
    {
        long long returned = ::std::strtoll(arg.c_str(), nullptr, 10);
        if (returned > ::std::numeric_limits<T>::max())
            return ::std::numeric_limits<T>::max();
        else if (returned < ::std::numeric_limits<T>::min())
            return ::std::numeric_limits<T>::min();
        else
            return static_cast<T>(returned);
    }

    template<> char FromString(const String &arg)
    {
        return FromStringCommon<char>(arg);
    }

    template<> signed char FromString(const String &arg)
    {
        return FromStringCommon<signed char>(arg);
    }

    template<> unsigned char FromString(const String &arg)
    {
        return FromStringCommon<unsigned char>(arg);
    }

    template<> short FromString(const String &arg)
    {
        return FromStringCommon<short>(arg);
    }

    template<> unsigned short FromString(const String &arg)
    {
        return FromStringCommon<unsigned short>(arg);
    }

    template<> bool FromString(const String &arg)
    {
        if (arg == "true")
            return true;
        else
            return false;
    }

    template<> LargeInteger FromString(const String &arg)
    {
        LargeInteger ret;
        ret.FromString(arg);
        return ret;
    }

    template<class T>
    String ToStringCharCommon(T arg)
    {
        return ToString(short(arg));
    }

    String ToString(char arg)
    {
        return ToStringCharCommon(arg);
    }

    String ToString(signed char arg)
    {
        return ToStringCharCommon(arg);
    }

    String ToString(unsigned char arg)
    {
        return ToStringCharCommon(arg);
    }

    String ToString(const String &arg)
    {
        return arg;
    }

    String ToString(bool arg)
    {
        if (arg)
            return "true";
        else
            return "false";
    }

    String ToString(LargeInteger arg)
    {
        return arg.ToString();
    }

    String ToString(TimeValue timeValue)
    {
        return ToString(timeValue.Get());
    }

    String ToString(GameTimeValue timeValue)
    {
        return ToString(timeValue.Get());
    }
}