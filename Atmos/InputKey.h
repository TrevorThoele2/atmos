#pragma once

#include "InputSignal.h"
#include "InputKeyID.h"
#include <AGUI/Input.h>

namespace Atmos
{
    namespace Input
    {
        class Manager;

        class Key : public Signal<KeyID, Agui::KeyID>
        {
        public:
            Key(Key&& arg);
        private:
            Key(
                Manager& owner,
                ObjectManager& objectManager,
                DataPtr&& data,
                KeyID id,
                Agui::KeyID guiID,
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
}