
#pragma once

#include <cmath>

namespace Atmos
{
    template<class T>
    const T pi = T(3.141592653589793);

    template<class T>
    const T fullCircleRadians = T(2 * pi<T>);

    template<class T>
    const T sqrt2 = T(1.414213562373095);

    template<class T>
    T Floor(T arg)
    {
        return std::floor(arg);
    }

    template<class T>
    T Ceiling(T arg)
    {
        return std::ceil(arg);
    }

    template<class T>
    T Round(T arg)
    {
        return static_cast<T>(Floor(arg + T(0.5)));
    }

    template<class T>
    T Square(T arg)
    {
        return arg * arg;
    }

    template<class Value, class Exp>
    Value Power(Value value, Exp exp)
    {
        if (exp < 0 || exp == 1)
            return value;

        if (exp == 0)
            return 1;

        Value ret = 1;
        while (exp-- > 0)
            ret *= value;
        return ret;
    }

    template<class T>
    T PowerOfTwo(T count)
    {
        return 1 << count;
    }

    inline float PowerOfTwo(float count)
    {
        return Power(2.0f, count);
    }

    inline double PowerOfTwo(double count)
    {
        return Power(2.0, count);
    }

    template<class T>
    T GreatestCommonDenominator(T num, T den)
    {
        if (den == 0)
            return num;
        else
            return GreatestCommonDenominator(den, num % den);
    }

    template<class T>
    T Percentage(T toConvert, T first, T second)
    {
        T range;
        if (second > first)
            range = second - first;
        else
            range = first - second;

        return (toConvert / range);
    }

    template<class T>
    T Midpoint(T left, T right)
    {
        return (right + left) / 2;
    }

    template<class T>
    inline T Modulo(T left, T right)
    {
        return left %= right;
    }

    template<>
    inline float Modulo(float left, float right)
    {
        return std::fmod(left, right);
    }

    template<>
    inline double Modulo(double left, double right)
    {
        return std::fmod(left, right);
    }

    template<class T>
    inline bool IsDivisibleBy(T value, T by)
    {
        return value % by == 0;
    }

    template<class T>
    inline bool IsEven(T t)
    {
        return IsDivisibleBy(t, 2);
    }

    template<class T>
    inline bool IsOdd(T t)
    {
        return !IsDivisibleBy(t, 2);
    }

    template<class From, class To>
    inline To ConvertRange(From fromValue, From fromMin, From fromMax, To toMin, To toMax)
    {
        const auto fromRange = static_cast<To>(fromMax - fromMin);
        if (fromRange == 0)
            return static_cast<To>(fromMin);
        else
        {
            const auto toRange = toMax - toMin;
            return (static_cast<To>((fromValue - fromMin) * toRange) / fromRange) + toMin;
        }
    }
}