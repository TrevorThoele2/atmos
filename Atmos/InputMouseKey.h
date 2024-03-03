#pragma once

#include "InputSignal.h"
#include "InputMouseKeyID.h"

namespace Atmos
{
    namespace Input
    {
        class MouseKey : public Signal<MouseKeyID>
        {
        public:
            MouseKey(MouseKey&& arg);
        private:
            MouseKey(
                Manager& owner,
                ObjectManager& objectManager,
                DataPtr&& data,
                MouseKeyID id,
                ::Agui::Input::Signal* guiSignal,
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