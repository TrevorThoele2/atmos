#pragma once

#include <limits>
#include <type_traits>

namespace Atmos
{
    // OVERFLOW ADD
    template<class T, typename std::enable_if<std::is_signed<T>::value, int>::type = 0>
    bool OverflowAdd(T left, T right, T max)
    {
        if (right < 0)
            return OverflowSubtract(left, -right, max);

        if (std::numeric_limits<T>::max() - right < left)
            return true;

        return max - right < left;
    }

    template<class T, typename std::enable_if<!std::is_signed<T>::value, int>::type = 0>
    bool OverflowAdd(T left, T right, T max)
    {
        if (std::numeric_limits<T>::max() - right < left)
            return true;

        return max - right < left;
    }

    template<class T, typename std::enable_if<std::is_signed<T>::value, int>::type = 0>
    bool UnderflowAdd(T left, T right, T min)
    {
        if (right < 0)
            return UnderflowSubtract(left, -right, min);

        if (-left > right - std::numeric_limits<T>::lowest())
            return true;

        return -left > right - min;
    }

    template<class T, typename std::enable_if<!std::is_signed<T>::value, int>::type = 0>
    bool UnderflowAdd(T left, T right, T min)
    {
        if (-left > right - std::numeric_limits<T>::lowest())
            return true;

        return -left > right - min;
    }

    template<class T, typename std::enable_if<std::is_signed<T>::value, int>::type = 0>
    bool OverflowSubtract(T left, T right, T max)
    {
        if (right < 0)
            return OverflowAdd(left, -right, max);

        if (std::numeric_limits<T>::max() - left < -right)
            return true;

        return max - left < -right;
    }

    template<class T, typename std::enable_if<!std::is_signed<T>::value, int>::type = 0>
    bool OverflowSubtract(T left, T right, T max)
    {
        if (std::numeric_limits<T>::max() - left < -right)
            return true;

        return max - left < -right;
    }

    template<class T, typename std::enable_if<std::is_signed<T>::value, int>::type = 0>
    bool UnderflowSubtract(T left, T right, T min)
    {
        if (right < 0)
            return UnderflowAdd(left, -right, min);

        if (std::numeric_limits<T>::lowest() + right > left)
            return true;

        return min + right > left;
    }

    template<class T, typename std::enable_if<!std::is_signed<T>::value, int>::type = 0>
    bool UnderflowSubtract(T left, T right, T min)
    {
        if (std::numeric_limits<T>::lowest() + right > left)
            return true;

        return min + right > left;
    }

    template<class T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    bool OverflowMultiply(T left, T right, T max)
    {
        if ((right < 0) ? std::numeric_limits<T>::max() / right > left : std::numeric_limits<T>::max() / right < left)
            return true;

        return (right < 0) ? max / right > left : max / right < left;
    }

    template<class T, typename std::enable_if<!std::is_integral<T>::value && std::is_floating_point<T>::value, int>::type = 0>
    bool OverflowMultiply(T left, T right, T max)
    {
        if (right < 1 && right > -1)
            return OverflowDivide(left, 1 / right, max);

        if ((right < 0) ? std::numeric_limits<T>::max() / right > left : std::numeric_limits<T>::max() / right < left)
            return true;

        return (right < 0) ? max / right > left : max / right < left;
    }

    template<class T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    bool UnderflowMultiply(T left, T right, T min)
    {
        if ((right < 0) ? std::numeric_limits<T>::lowest() / right < left : std::numeric_limits<T>::lowest() / right > left)
            return true;

        return (right < 0) ? min / right < left : min / right > left;
    }

    template<class T, typename std::enable_if<!std::is_integral<T>::value && std::is_floating_point<T>::value, int>::type = 0>
    bool UnderflowMultiply(T left, T right, T min)
    {
        if (right < 1 && right > -1)
            return UnderflowDivide(left, 1 / right, min);

        if ((right < 0) ? std::numeric_limits<T>::lowest() / right < left : std::numeric_limits<T>::lowest() / right > left)
            return true;

        return (right < 0) ? min / right < left : min / right > left;
    }

    template<class T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    bool OverflowDivide(T left, T right, T max)
    {
        return false;
    }

    template<class T, typename std::enable_if<!std::is_integral<T>::value && std::is_floating_point<T>::value, int>::type = 0>
    bool OverflowDivide(T left, T right, T max)
    {
        if (right < 1 && right > -1)
            return OverflowMultiply(left, 1 / right, max);

        if ((right < 0) ? std::numeric_limits<T>::max() * right > left : std::numeric_limits<T>::max() < left / right)
            return true;

        return (right < 0) ? max * right > left : max < left / right;
    }

    template<class T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    bool UnderflowDivide(T left, T right, T min)
    {
        return false;
    }

    template<class T, typename std::enable_if<!std::is_integral<T>::value && std::is_floating_point<T>::value, int>::type = 0>
    bool UnderflowDivide(T left, T right, T min)
    {
        if (right < 1 && right > -1)
            return UnderflowMultiply(left, 1 / right, min);

        if ((right < 0) ? std::numeric_limits<T>::lowest() * right < left : std::numeric_limits<T>::lowest() > left / right)
            return true;

        return (right < 0) ? min * right < left : min > left / right;
    }
}