
#pragma once

#include "String.h"

#include <AGUI/Root.h>

namespace Atmos
{
    class State;
    class ObjectManager;

    class StateGui
    {
    public:
        virtual ~StateGui() = 0;

        void Initialize();
        void Update();

        void Show();
        void Hide();

        Agui::Root* Root();
    protected:
        ObjectManager* const objectManager;
    protected:
        StateGui(ObjectManager& objectManager, const String& rootName);
    private:
        String rootName;
        Agui::Root* root;
    private:
        virtual void DoInitialize() = 0;
        virtual void DoUpdate();
        virtual void DoShow();
        virtual void DoHide();
    };
}