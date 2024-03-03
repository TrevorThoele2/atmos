#pragma once

#include "InputSignal.h"
#include "InputKeyID.h"

namespace Atmos::Input
{
    class Manager;

    class Key : public Signal<KeyID>
    {
    public:
        Key(Key&& arg);
    private:
        Key(
            Manager& owner,
            ObjectManager& objectManager,
            DataPtr&& data,
            KeyID id,
            ::Agui::Input::Signal* guiSignal,
            const String& displayName,
            bool canUseForAction);

        void DoActiveImpl() override;
        void DoUpImpl() override;
        void DoDownImpl() override;
        void DoDoubleDownImpl() override;

        bool IsKey() const override;
        bool IsMouseKey() const override;
    private:
        friend Manager;
    };
}