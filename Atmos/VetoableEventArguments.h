#pragma once

#include "Object.h"

namespace Atmos
{
    template<class T>
    class VetoableEventArguments
    {
    public:
        typedef T Arguments;
    public:
        VetoableEventArguments(Arguments arguments);

        void Veto();
        bool WillVeto() const;

        Arguments Focused() const;
    private:
        bool veto;
        Arguments arguments;
    };

    template<class T>
    VetoableEventArguments<T>::VetoableEventArguments(Arguments arguments) : veto(false), arguments(arguments)
    {}

    template<class T>
    void VetoableEventArguments<T>::Veto()
    {
        veto = true;
    }

    template<class T>
    bool VetoableEventArguments<T>::WillVeto() const
    {
        return veto;
    }

    template<class T>
    typename VetoableEventArguments<T>::Arguments VetoableEventArguments<T>::Focused() const
    {
        return arguments;
    }
}