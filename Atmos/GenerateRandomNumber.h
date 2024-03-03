#pragma once

#include <Arca/Command.h>
#include "RandomInformation.h"
#include "PrimitiveTraits.h"

namespace Atmos::Random
{
    template<class T>
    struct GenerateNumber
    {
        Range<T> range;

        GenerateNumber();
        GenerateNumber(Range<T> range);

        T Do(Arca::Reliquary& reliquary) const
        {
            const auto information = reliquary.Find<Information>();
            return information->random.Generate(range);
        }
    };

    template <class T>
    GenerateNumber<T>::GenerateNumber() : range(std::numeric_limits<T>::min(), std::numeric_limits<T>::max())
    {}

    template<class T>
    GenerateNumber<T>::GenerateNumber(Range<T> range) : range(range)
    {}

    struct GenerateBool
    {
        GenerateBool() = default;

        bool Do(Arca::Reliquary& reliquary) const;
    };
}

namespace Arca
{
    template<class T>
    struct Traits<Atmos::Random::GenerateNumber<T>>
    {
        static const ObjectType objectType = ObjectType::Command;
        static TypeName TypeName() { return "Atmos::Random::GenerateNumber<" + Atmos::PrimitiveTraits<T>::TypeName() + ">"; }
        using Result = T;
        static const bool selfContained = true;
    };

    template<>
    struct Traits<Atmos::Random::GenerateBool>
    {
        static const ObjectType objectType = ObjectType::Command;
        static TypeName TypeName() { return "Atmos::Random::GenerateBool"; }
        using Result = bool;
        static const bool selfContained = true;
    };
}