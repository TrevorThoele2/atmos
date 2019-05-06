
#pragma once

#include <cmath>

namespace Atmos
{
    // Trig constants
    template<class T>
    const T PI = T(3.141592653589793);

    template<class T>
    const T FULL_CIRCLE_RADIANS = T(2 * PI<T>);

    // Square root constants
    template<class T>
    const T SQRT_2 = T(1.414213562373095);

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

    // Will do 2 ^ count
    template<class T>
    T PowerOfTwo(T count)
    {
        return 1 << count;
    }

    template<class T>
    T GCD(T num, T den)
    {
        if (den == 0)
            return num;
        else
            return GCD(den, num % den);
    }

    template<class T>
    T GetPercentage(T toConvert, T first, T second)
    {
        T range;
        if (second > first)
            range = second - first;
        else
            range = first - second;

        return (toConvert / range);
    }

    template<class T>
    T GetMidpoint(T left, T right)
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
    inline bool IsEven(T t)
    {
        return t % 2 == 0;
    }

    template<class T>
    inline bool IsOdd(T t)
    {
        return t % 2 == 1;
    }
}