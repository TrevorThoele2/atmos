#pragma once

#include <limits>

#include "FixedPoint.h"

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

    template<class T>
    bool OverflowAdd(const FixedPoint<T> &left, const FixedPoint<T> &right, const FixedPoint<T> &max)
    {
        typedef FixedPoint<T> FixedPointT;
        typedef typename FixedPointT::Split Split;

        if (right < Split(0, 0))
            return OverflowSubtract(left, -right, max);

        if (FixedPointT(FixedPointT::GetMax(), left.GetRadixPoint()) - right < left)
            return true;

        return max - right < left;
    }

    // END OVERFLOW ADD
    // UNDERFLOW ADD

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

    template<class T>
    bool UnderflowAdd(const FixedPoint<T> &left, const FixedPoint<T> &right, const FixedPoint<T> &min)
    {
        typedef FixedPoint<T> FixedPointT;
        typedef typename FixedPointT::Split Split;

        if (right < Split(0, 0))
            return UnderflowSubtract(left, -right, min);

        if (-left > right - FixedPointT(FixedPointT::GetLowest(), left.GetRadixPoint()))
            return true;

        return -left > right - min;
    }

    // END UNDERFLOW ADD
    // OVERFLOW SUBTRACT

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

    template<class T>
    bool OverflowSubtract(const FixedPoint<T> &left, const FixedPoint<T> &right, const FixedPoint<T> &max)
    {
        typedef FixedPoint<T> FixedPointT;
        typedef typename FixedPointT::Split Split;

        if (right < Split(0, 0))
            return OverflowAdd(left, -right, max);

        if (FixedPointT(FixedPointT::GetMax(), left.GetRadixPoint()) - left < -right)
            return true;

        return max - left < -right;
    }

    // END OVERFLOW SUBTRACT
    // UNDERFLOW SUBTRACT

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

    template<class T>
    bool UnderflowSubtract(const FixedPoint<T> &left, const FixedPoint<T> &right, const FixedPoint<T> &min)
    {
        typedef FixedPoint<T> FixedPointT;
        typedef typename FixedPointT::Split Split;

        if (right < Split(0, 0))
            return UnderflowAdd(left, -right, min);

        if (FixedPointT(FixedPointT::GetLowest(), left.GetRadixPoint()) + right > left)
            return true;

        return min + right > left;
    }

    // END UNDERFLOW SUBTRACT
    // OVERFLOW MULTIPLY

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

    template<class T>
    bool OverflowMultiply(const FixedPoint<T> &left, const FixedPoint<T> &right, const FixedPoint<T> &max)
    {
        typedef FixedPoint<T> FixedPointT;
        typedef typename FixedPointT::Split Split;

        if (right < Split(1, 0) && right > Split(-1, 0))
            return OverflowDivide(left, FixedPointT(Split(1, 0) / right), max);

        if ((right < Split(0, 0)) ? FixedPointT(FixedPointT::GetMax(), left.GetRadixPoint()) / right > left : FixedPointT(FixedPointT::GetMax(), left.GetRadixPoint()) / right < left)
            return true;

        return (right < Split(0, 0)) ? max / right > left : max / right < left;
    }

    // END OVERFLOW MULTIPLY
    // UNDERFLOW MULTIPLY

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

    template<class T>
    bool UnderflowMultiply(const FixedPoint<T> &left, const FixedPoint<T> &right, const FixedPoint<T> &min)
    {
        typedef FixedPoint<T> FixedPointT;
        typedef typename FixedPointT::Split Split;

        if (right < Split(1, 0) && right > Split(-1, 0))
            return UnderflowDivide(left, FixedPointT(Split(1, 0) / right), min);

        if ((right < Split(0, 0)) ? FixedPointT(FixedPointT::GetLowest(), left.GetRadixPoint()) / right < left : FixedPointT(FixedPointT::GetLowest(), left.GetRadixPoint()) / right > left)
            return true;

        return (right < Split(0, 0)) ? min / right < left : min / right > left;
    }

    // END UNDERFLOW MULTIPLY
    // OVERFLOW DIVIDE

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

    template<class T>
    bool OverflowDivide(const FixedPoint<T> &left, const FixedPoint<T> &right, const FixedPoint<T> &min)
    {
        typedef FixedPoint<T> FixedPointT;
        typedef typename FixedPointT::Split Split;

        if (right < Split(1, 0) && right > Split(-1, 0))
            return OverflowMultiply(left, FixedPointT(Split(1, 0) / right), min);

        if ((right < Split(0, 0)) ? FixedPointT(FixedPointT::GetMax(), left.GetRadixPoint()) * right > left : FixedPointT(FixedPointT::GetMax(), left.GetRadixPoint()) < left / right)
            return true;

        return (right < Split(0, 0)) ? min * right > left : min < left / right;
    }

    // END OVERFLOW DIVIDE
    // UNDERFLOW DIVIDE

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

    template<class T>
    bool UnderflowDivide(const FixedPoint<T> &left, const FixedPoint<T> &right, const FixedPoint<T> &min)
    {
        typedef FixedPoint<T> FixedPointT;
        typedef typename FixedPointT::Split Split;

        if (right < Split(1, 0) && right > Split(-1, 0))
            return UnderflowMultiply(left, FixedPointT(Split(1, 0) / right), min);

        if ((right < Split(0, 0)) ? FixedPointT(FixedPointT::GetLowest(), left.GetRadixPoint()) * right < left : FixedPointT(FixedPointT::GetLowest(), left.GetRadixPoint()) > left / right)
            return true;

        return (right < Split(0, 0)) ? min * right < left : min > left / right;
    }

    // END UNDERFLOW DIVIDE
}