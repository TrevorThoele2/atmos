#include "StateGui.h"

namespace Atmos::State
{
    StateGui::~StateGui()
    {}

    void StateGui::Initialize()
    {
        DoInitialize();
    }

    void StateGui::Update()
    {
        DoUpdate();
    }

    void StateGui::Show()
    {
        DoShow();
    }

    void StateGui::Hide()
    {
        DoHide();
    }

    StateGui::StateGui(ObjectManager& objectManager, const String& rootName) :
        objectManager(&objectManager), rootName(rootName)
    {}

    void StateGui::DoUpdate()
    {}

    void StateGui::DoShow()
    {}

    void StateGui::DoHide()
    {}
}