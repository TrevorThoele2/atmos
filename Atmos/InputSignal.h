#pragma once

#include "InputSignalBase.h"

namespace Atmos
{
    class ObjectManager;

    namespace Input
    {
        class Manager;
        
        template<class T, class GuiId>
        class Signal : public SignalBase
        {
        public:
            T id;
            GuiId guiId;
        protected:
            Signal(Manager& owner, ObjectManager& objectManager, DataPtr&& data, T id, GuiId guiId, const String& displayName, bool canUseForAction);
            Signal(Signal&& arg);
        };

        template<class T, class GuiId>
        Signal<T, GuiId>::Signal(
            Manager& owner,
            ObjectManager& objectManager,
            DataPtr&& data,
            T id,
            GuiId guiId,
            const String& displayName,
            bool canUseForAction) :

            SignalBase(owner, objectManager, std::move(data), displayName, canUseForAction), id(id), guiId(guiId)
        {}

        template<class T, class GuiId>
        Signal<T, GuiId>::Signal(Signal&& arg) : SignalBase(std::move(arg))
        {}
    }
}