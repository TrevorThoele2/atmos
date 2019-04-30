#pragma once

#include "InputSignal.h"
#include "InputMouseKeyID.h"
#include <AGUI/Input.h>

namespace Atmos
{
    namespace Input
    {
        class MouseKey : public Signal<MouseKeyID, Agui::MouseKeyID>
        {
        public:
            MouseKey(MouseKey&& arg);
        private:
            MouseKey(
                Manager& owner,
                ObjectManager& objectManager,
                DataPtr&& data,
                MouseKeyID id,
                Agui::MouseKeyID guiID,
                const String& displayName);

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