
#pragma once

#include <random>
#include <type_traits>

#include "NumericLimits.h"
#include "FixedPoint.h"
#include "Range.h"

namespace Atmos
{
    class Random
    {
    public:
        Random();
        Random(const Random& arg) = default;
        Random& operator=(const Random& arg) = default;

        bool GenerateBool(double probability = 0.5);
        template<class T>
        T Generate();
        template<class T>
        T Generate(T floor, T ceiling);
        template<class T>
        FixedPoint<T> Generate(FixedPoint<T> floor, FixedPoint<T> ceiling);
        template<class T>
        T Generate(const Range<T>& range);
    private:
        std::default_random_engine randomEngine;

        char GenerateImpl(char floor, char ceiling);
        unsigned char GenerateImpl(unsigned char floor, unsigned char ceiling);
        signed char GenerateImpl(signed char floor, signed char ceiling);
        short GenerateImpl(short floor, short ceiling);
        unsigned short GenerateImpl(unsigned short floor, unsigned short ceiling);
        int GenerateImpl(int floor, int ceiling);
        unsigned int GenerateImpl(unsigned int floor, unsigned int ceiling);
        long long GenerateImpl(long long floor, long long ceiling);
        unsigned long long GenerateImpl(unsigned long long floor, unsigned long long ceiling);
        float GenerateImpl(float floor, float ceiling);
        double GenerateImpl(double floor, double ceiling);
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
    T Random::Generate(const Range<T>& range)
    {
        return Generate(range.GetBegin(), range.GetEnd());
    }
}