
#include "InputKey.h"

#include "InputManager.h"

#include <AGUI/System.h>

namespace Atmos
{
    namespace Input
    {
        Key::Key(Key&& arg) : Signal(std::move(arg))
        {}

        Key::Key(
            Manager& owner,
            ObjectManager& objectManager,
            DataPtr&& data,
            KeyID id,
            Agui::KeyID guiID,
            const String& displayName,
            bool canUseForAction) :

            Signal(owner, objectManager, std::move(data), id, guiID, displayName, canUseForAction)
        {}

        void Key::DoActiveImpl()
        {
            Owner()->eventKeys.active(*this);
            Agui::System::InjectInputActive(guiId);
        }

        void Key::DoUpImpl()
        {
            Owner()->eventKeys.pressed(*this);
            ::Agui::System::InjectInputUp(guiId);
        }

        void Key::DoDownImpl()
        {
            Owner()->eventKeys.depressed(*this);
            ::Agui::System::InjectInputDown(guiId);
        }

        void Key::DoDoubleDownImpl()
        {
            Owner()->eventKeys.doublePressed(*this);
        }

        bool Key::IsKey() const
        {
            return true;
        }

        bool Key::IsMouseKey() const
        {
            return false;
        }
    }
}