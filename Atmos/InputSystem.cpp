
#include "InputSystem.h"

#include "NullInput.h"

#include "ObjectManager.h"
#include "InitializationFileSystem.h"

namespace Atmos::Input
{
    InputSystem::InputSystem(ObjectManager& manager) : UniqueProviderSystem(manager, UniqueProviderSystem::ValuePtr(new NullManager()))
    {}

    void InputSystem::InitializeImpl()
    {
        Get()->Initialize();

        auto initialization = Manager()->FindSystem<InitializationFileSystem>();
        auto fabricateEntry = [this, initialization](const String& name, KeyID keyID, ActionID actionID)
        {
            auto entry = initialization->controls.CreateEntry<Input::KeyID>(name, [this, actionID]()
            {
                auto mappedSignal = this->Get()->GetAction(actionID)->MappedSignal();
                return static_cast<const Input::Key*>(mappedSignal)->id;
            });
            entry->onLoaded.Subscribe([this, actionID](Input::KeyID value) {
                auto action = this->Get()->GetAction(actionID);
                auto key = this->Get()->GetKey(value);
                action->MapToSignal(key);
            });
        };

        fabricateEntry("Move Left", Input::KeyID::A, Input::ActionID::MOVE_LEFT);
        fabricateEntry("Move Up", Input::KeyID::W, Input::ActionID::MOVE_UP);
        fabricateEntry("Move Right", Input::KeyID::D, Input::ActionID::MOVE_RIGHT);
        fabricateEntry("Move Down", Input::KeyID::S, Input::ActionID::MOVE_DOWN);
        fabricateEntry("Use", Input::KeyID::E, Input::ActionID::USE);
        fabricateEntry("Menu Left", Input::KeyID::LEFT_ARROW, Input::ActionID::NAVIGATE_MENU_LEFT);
        fabricateEntry("Menu Up", Input::KeyID::UP_ARROW, Input::ActionID::NAVIGATE_MENU_UP);
        fabricateEntry("Menu Right", Input::KeyID::RIGHT_ARROW, Input::ActionID::NAVIGATE_MENU_RIGHT);
        fabricateEntry("Menu Down", Input::KeyID::DOWN_ARROW, Input::ActionID::NAVIGATE_MENU_DOWN);
        fabricateEntry("Select Character Left", Input::KeyID::L_BRACKET, Input::ActionID::CHANGE_SELECTED_CHARACTER_LEFT);
        fabricateEntry("Select Character Right", Input::KeyID::R_BRACKET, Input::ActionID::CHANGE_SELECTED_CHARACTER_RIGHT);
        fabricateEntry("Inventory", Input::KeyID::I, Input::ActionID::INVENTORY);
        fabricateEntry("End Turn", Input::KeyID::END, Input::ActionID::END_TURN);
        fabricateEntry("Attack", Input::KeyID::Q, Input::ActionID::ATTACK);
        fabricateEntry("Spells", Input::KeyID::O, Input::ActionID::OPEN_SPELLS);
        fabricateEntry("Cancel", Input::KeyID::C, Input::ActionID::INVENTORY);
        fabricateEntry("Stats", Input::KeyID::U, Input::ActionID::STATS);
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Input::InputSystem, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::ObjectSystem>(object, archive);
    }
}