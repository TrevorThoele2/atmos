#include "StringUtility.h"

#include "GridPosition.h"

#include "FilePath.h"

namespace Atmos
{
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

    void Trim(String &trim)
    {
        while (trim[0] == ' ' || trim[0] == '\n')
            trim.erase(0, 1);

        while (trim[trim.size() - 1] == ' ' || trim[trim.size() - 1] == '\n')
            trim.erase(trim.size() - 1, 1);
    }

    String Trim(const String& trim)
    {
        String ret = trim;

        while (ret[ret.size() - 1] == ' ' || ret[ret.size() - 1] == '\n')
        {
            ret.erase(ret.size() - 1, 1);

            if (ret.empty())
                return ret;
        }

        while (ret[0] == ' ' || ret[0] == '\n')
            ret.erase(0, 1);

        return ret;
    }

    bool IsAllWhitespace(const String& check)
    {
        if (check.empty())
            return false;

        for (auto &c : check)
        {
            if (c != ' ' && c != '\n')
                return false;
        }

        return true;
    }

    bool StartsWith(const String& check, const String& startsWith)
    {
        return check.find_first_of(startsWith) == 0;
    }

    namespace Detail
    {
        String FromStringImpl(const String &arg, const ::Chroma::TypeIdentity<String> &t)
        {
            return arg;
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

    template<> ::Agui::Resolution::Size FromString(const String &arg)
    {
        ::Agui::Resolution::Size size;
        const auto pivot = arg.find_first_of("x");
        const auto left = arg.substr(0, pivot);
        const auto right = arg.substr(pivot + 1);

        size.width = FromString<::Agui::Resolution::Size::Value>(left);
        size.height = FromString<::Agui::Resolution::Size::Value>(right);
        return size;
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

    String ToString(const Grid::Position &position)
    {
        return
            ToString(position.x) + "," +
            ToString(position.y) + ",";
    }

    String ToString(const Position3D& position)
    {
        return
            ToString(position.x) + "," +
            ToString(position.y) + "," +
            ToString(position.z);
    }

    String ToString(const Size2D& size)
    {
        return ToString(size.width) + "," + ToString(size.height);
    }

    String ToString(const ::Agui::Resolution::Size &arg)
    {
        return ToString(arg.width) + "x" + ToString(arg.height);
    }
}