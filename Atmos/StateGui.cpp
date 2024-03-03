
#include "StateGui.h"

#include <AGUI\System.h>

namespace Atmos
{
    void StateGui::UpdateImpl()
    {}

    void StateGui::ShowImpl()
    {}

    void StateGui::HideImpl()
    {}

    StateGui::StateGui(const std::string &name) : rootName(name), root(nullptr)
    {}

    void StateGui::Init()
    {
        root = Agui::System::CreateRoot(rootName);
        InitImpl();
    }

    void StateGui::Update()
    {
        UpdateImpl();
    }

    void StateGui::Show()
    {
        root->Show();
        ShowImpl();
    }

    void StateGui::Hide()
    {
        root->Hide();
        HideImpl();
    }

    Agui::Root* StateGui::GetRoot()
    {
        return root;
    }
}