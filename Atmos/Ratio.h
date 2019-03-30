
#pragma once

#include "Helper.h"

#include "Serialization.h"
#include <Inscription\Inscripter.h>

namespace Atmos
{
    template<class T>
    class RatioBase
    {
    public:
        typedef T ValueT;
        typedef double Divided;
    public:
        virtual ~RatioBase() = 0 {}
        virtual ValueT GetNumerator() const = 0;
        virtual ValueT GetDenominator() const = 0;
    };

    template<class T>
    class DynamicRatio : public RatioBase<T>
    {
    public:
        typedef RatioBase<T>::ValueT ValueT;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        ValueT numerator, denominator;
        bool simplify;
        void AttemptSimplify();
    public:
        DynamicRatio(ValueT numerator = 1, ValueT denominator = 1, bool simplify = true);
        bool operator==(const DynamicRatio &arg) const;
        void Set(ValueT num, ValueT den);
        void SetNumerator(ValueT set);
        void SetDenominator(ValueT set);
        void SetSimplify(bool set);
        ValueT GetNumerator() const override;
        ValueT GetDenominator() const override;
    };

    template<class T>
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(DynamicRatio<T>)
    {
        scribe(numerator);
        scribe(denominator);
        scribe(simplify);
    }

    template<class T>
    void DynamicRatio<T>::AttemptSimplify()
    {
        if (!simplify || denominator == 0)
            return;

        auto result = GCD(numerator, denominator);
        numerator /= result;
        denominator /= result;
    }

    template<class T>
    DynamicRatio<T>::DynamicRatio(ValueT numerator, ValueT denominator, bool simplify) : numerator(numerator), denominator(denominator)
    {
        AttemptSimplify();
    }

    template<class T>
    bool DynamicRatio<T>::operator==(const DynamicRatio &arg) const
    {
        return numerator == arg.numerator && denominator == arg.denominator && simplify == arg.simplify;
    }

    template<class T>
    void DynamicRatio<T>::Set(ValueT num, ValueT den)
    {
        numerator = num;
        denominator = den;
        AttemptSimplify();
    }

    template<class T>
    void DynamicRatio<T>::SetNumerator(ValueT set)
    {
        numerator = set;
        AttemptSimplify();
    }

    template<class T>
    void DynamicRatio<T>::SetDenominator(ValueT set)
    {
        denominator = set;
        AttemptSimplify();
    }

    template<class T>
    void DynamicRatio<T>::SetSimplify(bool set)
    {
        denominator = set;
        AttemptSimplify();
    }

    template<class T>
    typename DynamicRatio<T>::ValueT DynamicRatio<T>::GetNumerator() const
    {
        return numerator;
    }

    template<class T>
    typename DynamicRatio<T>::ValueT DynamicRatio<T>::GetDenominator() const
    {
        return denominator;
    }

    template<class T, T numerator, T denominator>
    class StaticRatio : public RatioBase<T>
    {
    public:
        typedef RatioBase<T>::ValueT ValueT;
    public:
        ValueT GetNumerator() const override;
        ValueT GetDenominator() const override;
    };

    template<class T, T numerator, T denominator>
    typename StaticRatio<T, numerator, denominator>::ValueT StaticRatio<T, numerator, denominator>::GetNumerator() const
    {
        return numerator;
    }

    template<class T, T numerator, T denominator>
    typename StaticRatio<T, numerator, denominator>::ValueT StaticRatio<T, numerator, denominator>::GetDenominator() const
    {
        return denominator;
    }
}