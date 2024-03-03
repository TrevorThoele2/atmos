
#pragma once

#include <random>
#include <type_traits>

#include "NumericLimits.h"
#include "FixedPoint.h"
#include "Range.h"

namespace Atmos
{
    namespace Detail
    {
        template<class T>
        class IsBool
        {
            static const bool value = false;
        };

        template<>
        class IsBool<bool>
        {
            static const bool value = true;
        };
    }

    class Random
    {
    private:
        static Random instance;

        std::default_random_engine randGen;

        Random();
        Random(const Random &arg) = delete;
        Random& operator=(const Random &arg) = delete;

        static char GenerateImpl(char floor, char ceiling);
        static unsigned char GenerateImpl(unsigned char floor, unsigned char ceiling);
        static signed char GenerateImpl(signed char floor, signed char ceiling);
        static short GenerateImpl(short floor, short ceiling);
        static unsigned short GenerateImpl(unsigned short floor, unsigned short ceiling);
        static int GenerateImpl(int floor, int ceiling);
        static unsigned int GenerateImpl(unsigned int floor, unsigned int ceiling);
        static long long GenerateImpl(long long floor, long long ceiling);
        static unsigned long long GenerateImpl(unsigned long long floor, unsigned long long ceiling);
        static float GenerateImpl(float floor, float ceiling);
        static double GenerateImpl(double floor, double ceiling);
    public:
        static Random& Instance();
        static bool GenerateBool(double probability = 0.5);
        template<class T>
        static T Generate();
        template<class T>
        static T Generate(T floor, T ceiling);
        template<class T>
        static FixedPoint<T> Generate(FixedPoint<T> floor, FixedPoint<T> ceiling);
        template<class T>
        static T Generate(const Range<T> &range);
    };

    template<class T>
    T Random::Generate()
    {
        return Generate(NumericLimits<T>::min(), NumericLimits<T>::max());
    }

    template<class T>
    T Random::Generate(T floor, T ceiling)
    {
        if (floor == ceiling)
            return floor;

        return GenerateImpl(floor, ceiling);
    }

    template<class T>
    FixedPoint<T> Random::Generate(FixedPoint<T> floor, FixedPoint<T> ceiling)
    {
        typedef FixedPoint<T> FixedT;
        if (floor == ceiling)
            return floor;

        return FixedT(FixedT::SplitValue(GenerateImpl(floor.GetRawValue(), ceiling.GetRawValue())));
    }

    template<class T>
    T Random::Generate(const Range<T> &range)
    {
        return Generate(range.GetBegin(), range.GetEnd());
    }
}