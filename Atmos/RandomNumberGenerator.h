#pragma once

#include <random>
#include "Range.h"
#include "RangeMinNotLessThanEqualMax.h"

namespace Atmos::Random
{
    class NumberGenerator
    {
    public:
        NumberGenerator();

        [[nodiscard]] bool GenerateBool() const;
        template<class T>
        [[nodiscard]] T Generate(Range<T> range) const;
    private:
        mutable std::default_random_engine randomEngine;

        template<class U>
        struct DistributionResolver
        {
            using Distribution = std::uniform_int_distribution<U>;
        };

        template<>
        struct DistributionResolver<std::int8_t>
        {
            using Distribution = std::uniform_int_distribution<std::int16_t>;
        };

        template<>
        struct DistributionResolver<std::uint8_t>
        {
            using Distribution = std::uniform_int_distribution<std::uint16_t>;
        };

        template<>
        struct DistributionResolver<float>
        {
            using Distribution = std::uniform_real_distribution<float>;
        };

        template<>
        struct DistributionResolver<double>
        {
            using Distribution = std::uniform_real_distribution<double>;
        };

        template<class T>
        using Distribution = typename DistributionResolver<T>::Distribution;
        template<class T>
        Distribution<T> ToDistribution(Range<T> range) const;

        template<class T>
        void ThrowIfRangeInvalid(Range<T> range) const;
    };

    template<class T>
    T NumberGenerator::Generate(Range<T> range) const
    {
        ThrowIfRangeInvalid(range);

        if (range.Distance() == 0)
            return range.min;

        auto distribution = ToDistribution(range);

        return static_cast<T>(distribution(randomEngine));
    }

    template<class T>
    auto NumberGenerator::ToDistribution(Range<T> range) const -> Distribution<T>
    {
        return Distribution<T>{ range.min, range.max };
    }

    template<class T>
    void NumberGenerator::ThrowIfRangeInvalid(Range<T> range) const
    {
        if (range.min > range.max)
            throw RangeMinNotLessThanEqualMax();
    }
}