
#pragma once

#include "Serialization.h"
#include <Inscription\Scribe.h>

namespace Atmos
{
    template<class T>
    struct Join2
    {
    public:
        T first, second;
        Join2() = default;
        Join2(const T &first, const T &second) : first(first), second(second) {}
        Join2(T &&first, T &&second) : first(std::move(first)), second(std::move(second)) {}

        Join2(const Join2 &arg) = default;
        Join2& operator=(const Join2 &arg) = default;

        Join2(Join2 &&arg) : first(std::move(arg.first)), second(std::move(arg.second)) {}
        Join2& operator=(Join2 &&arg)
        {
            first = std::move(arg.first);
            second = std::move(arg.second);
            return *this;
        }

        bool operator==(const Join2 &arg) const
        {
            return first == arg.first && second == arg.second;
        }

        bool operator!=(const Join2 &arg) const
        {
            return (*this == arg);
        }
    private:
        INSCRIPTION_SERIALIZE_FUNCTION{ scribe(first, second); }
        INSCRIPTION_ACCESS;
    };

    template<class T>
    struct Join3
    {
    public:
        T first, second, third;
        Join3() = default;
        Join3(const T &first, const T &second, const T &third) : first(first), second(second), third(third) {}
        Join3(T &&first, T &&second, T &&third) : first(std::move(first)), second(std::move(second)), third(std::move(third)) {}

        Join3(const Join3 &arg) = default;
        Join3& operator=(const Join3 &arg) = default;

        Join3(Join3 &&arg) : first(std::move(arg.first)), second(std::move(arg.second)), third(std::move(arg.third)) {}
        Join3& operator=(Join3 &&arg)
        {
            first = std::move(arg.first);
            second = std::move(arg.second);
            third = std::move(arg.third);
            return *this;
        }

        bool operator==(const Join3 &arg) const
        {
            return first == arg.first && second == arg.second && third == arg.third;
        }

        bool operator!=(const Join3 &arg) const
        {
            return !(*this == arg);
        }
    private:
        INSCRIPTION_SERIALIZE_FUNCTION{ scribe(first, second, third); }
        INSCRIPTION_ACCESS;
    };

    template<class T>
    struct Join4
    {
    public:
        T first, second, third, fourth;
        Join4() = default;
        Join4(const T &first, const T &second, const T &third, const T &fourth) : first(first), second(second), third(third), fourth(fourth) {}
        Join4(T &&first, T &&second, T &&third, T &&fourth) : first(std::move(first)), second(std::move(second)), third(std::move(third)), fourth(std::move(fourth)) {}

        Join4(const Join4 &arg) = default;
        Join4& operator=(const Join4 &arg) = default;

        Join4(Join4 &&arg) : first(std::move(arg.first)), second(std::move(arg.second)), third(std::move(arg.third)), fourth(std::move(arg.fourth)) {}
        Join4& operator=(Join4 &&arg)
        {
            first = std::move(arg.first);
            second = std::move(arg.second);
            third = std::move(arg.third);
            fourth = std::move(arg.fourth);
            return *this;
        }

        bool operator==(const Join4 &arg) const
        {
            return first == arg.first && second == arg.second && third == arg.third && fourth == arg.fourth;
        }

        bool operator!=(const Join4 &arg) const
        {
            return !(*this == arg);
        }
    private:
        INSCRIPTION_SERIALIZE_FUNCTION{ scribe(first, second, third, fourth); }
        INSCRIPTION_ACCESS;
    };
}