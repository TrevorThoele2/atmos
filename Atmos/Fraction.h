#pragma once

namespace Atmos
{
    template<class T>
    struct Fraction
    {
        T numerator;
        T denominator;
        Fraction(T numerator, T denominator);
        Fraction(const Fraction &arg);
        Fraction(Fraction &&arg);
        Fraction& operator=(const Fraction &arg);
        Fraction& operator=(Fraction &&arg);
    };

    template<class T>
    Fraction<T>::Fraction(T numerator, T denominator) : numerator(numerator), denominator(denominator)
    {}

    template<class T>
    Fraction<T>::Fraction(const Fraction &arg) : numerator(arg.numerator), denominator(arg.denominator)
    {}

    template<class T>
    Fraction<T>::Fraction(Fraction &&arg) : numerator(arg.numerator), denominator(arg.denominator)
    {}

    template<class T>
    Fraction<T>& Fraction<T>::operator=(const Fraction &arg)
    {
        numerator = arg.numerator;
        denominator = arg.denominator;
        return *this;
    }

    template<class T>
    Fraction<T>& Fraction<T>::operator=(Fraction &&arg)
    {
        numerator = arg.numerator;
        denominator = arg.denominator;
        return *this;
    }
}