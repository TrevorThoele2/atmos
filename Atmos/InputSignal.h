#pragma once

#include "InputSignalBase.h"
#include <AGUI/InputSignal.h>

namespace Atmos
{
    class ObjectManager;
}

namespace Atmos::Input
{
    class Manager;
        
    template<class T>
    class Signal : public SignalBase
    {
    public:
        T id;
        Agui::Input::Signal* guiSignal;
    protected:
        Signal(
            Manager& owner,
            ObjectManager& objectManager,
            DataPtr&& data,
            T id,
            ::Agui::Input::Signal* guiSignal,
            const String& displayName,
            bool canUseForAction);
        Signal(Signal&& arg);
    };

    template<class T>
    Signal<T>::Signal(
        Manager& owner,
        ObjectManager& objectManager,
        DataPtr&& data,
        T id,
        ::Agui::Input::Signal* guiSignal,
        const String& displayName,
        bool canUseForAction) :

        SignalBase(owner, objectManager, std::move(data), displayName, canUseForAction),
        id(id), guiSignal(guiSignal)
    {}

    template<class T>
    Signal<T>::Signal(Signal&& arg) : SignalBase(std::move(arg))
    {}
}
