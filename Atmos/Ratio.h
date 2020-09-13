#pragma once

#include "Serialization.h"

namespace Atmos
{
    template<class T>
    class Ratio
    {
    public:
        using Value = T;
        using Divided = double;
    public:
        virtual ~Ratio() = 0;

        virtual Value Numerator() const = 0;
        virtual Value Denominator() const = 0;
    };

    template<class T>
    Ratio<T>::~Ratio()
    {}

    template<class T>
    class DynamicRatio : public Ratio<T>
    {
    public:
        typedef Ratio<T>::Value Value;

        Value numerator;
        Value denominator;
    public:
        DynamicRatio();
        DynamicRatio(Value numerator, Value denominator);

        bool operator==(const DynamicRatio& arg) const;

        void Simplify();

        void Numerator(Value set);
        void Denominator(Value set);
        Value Numerator() const override;
        Value Denominator() const override;
    private:
        INSCRIPTION_ACCESS;
    };

    template<class T>
    DynamicRatio<T>::DynamicRatio() : numerator(1), denominator(1)
    {}

    template<class T>
    DynamicRatio<T>::DynamicRatio(Value numerator, Value denominator) : numerator(numerator), denominator(denominator)
    {}

    template<class T>
    bool DynamicRatio<T>::operator==(const DynamicRatio& arg) const
    {
        return numerator == arg.numerator && denominator == arg.denominator;
    }

    template<class T>
    void DynamicRatio<T>::Simplify()
    {
        if (denominator == 0)
            return;

        auto result = GreatestCommonDenominator(numerator, denominator);
        numerator /= result;
        denominator /= result;
    }

    template<class T>
    void DynamicRatio<T>::Numerator(Value set)
    {
        numerator = set;
    }

    template<class T>
    void DynamicRatio<T>::Denominator(Value set)
    {
        denominator = set;
    }

    template<class T>
    typename DynamicRatio<T>::Value DynamicRatio<T>::Numerator() const
    {
        return numerator;
    }

    template<class T>
    typename DynamicRatio<T>::Value DynamicRatio<T>::Denominator() const
    {
        return denominator;
    }

    template<class T, T numerator, T denominator>
    class StaticRatio : public Ratio<T>
    {
    public:
        using Value = Ratio<T>::Value;
    public:
        Value Numerator() const override;
        Value Denominator() const override;
    };

    template<class T, T numerator, T denominator>
    auto StaticRatio<T, numerator, denominator>::Numerator() const -> Value
    {
        return numerator;
    }

    template<class T, T numerator, T denominator>
    auto StaticRatio<T, numerator, denominator>::Denominator() const -> Value
    {
        return denominator;
    }
}

namespace Inscription
{
    template<class T>
    class Scribe<::Atmos::DynamicRatio<T>, BinaryArchive> : public CompositeScribe<::Atmos::DynamicRatio<T>, BinaryArchive>
    {
    private:
        using BaseT = CompositeScribe<::Atmos::DynamicRatio<T>, BinaryArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive)
        {
            archive(object.numerator);
            archive(object.denominator);
        }
    };
}