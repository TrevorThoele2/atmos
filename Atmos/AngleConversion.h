#pragma once

#include "Degrees.h"
#include "Radians.h"

namespace Atmos
{
    template<class To>
    class AngleConverterBase
    {
    public:
        typedef float ValueT;
        typedef Radians FromT;
        typedef To ToT;
    };

    template<class To>
    class AngleConverter;

    template<>
    class AngleConverter<Radians> : public AngleConverterBase<Radians>
    {
    public:
        static ToT To(FromT from);
        static FromT From(ToT from);
    };

    template<>
    class AngleConverter<Degrees> : public AngleConverterBase<Degrees>
    {
    public:
        static ToT To(FromT from);
        static FromT From(ToT from);
    };
}