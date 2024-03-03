
#include "StateGui.h"

#include <AGUI/System.h>

namespace Atmos
{
    StateGui::~StateGui()
    {}

    void StateGui::Initialize()
    {
        root = Agui::System::CreateRoot(rootName);
        DoInitialize();
    }

    void StateGui::Update()
    {
        DoUpdate();
    }

    void StateGui::Show()
    {
        root->Show();
        DoShow();
    }

    void StateGui::Hide()
    {
        root->Hide();
        DoHide();
    }

    Agui::Root* StateGui::Root()
    {
        return root;
    }

    StateGui::StateGui(ObjectManager& objectManager, const String& rootName) :
        objectManager(&objectManager), rootName(rootName), root(nullptr)
    {}

    void StateGui::DoUpdate()
    {}

    void StateGui::DoShow()
    {}

    void StateGui::DoHide()
    {}
}