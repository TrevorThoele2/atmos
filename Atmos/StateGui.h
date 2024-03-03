
#pragma once

#include <string>

#include <AGUI\Root.h>

namespace Atmos
{
    class StateGui
    {
    private:
        std::string rootName;
        agui::Root *root;
        virtual void InitImpl() = 0;
        virtual void UpdateImpl();
        virtual void ShowImpl();
        virtual void HideImpl();
    protected:
        StateGui(const std::string &name);
        agui::Root* GetRoot();
    public:
        void Init();
        void Update();
        void Show();
        void Hide();
    };
}