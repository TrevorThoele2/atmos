
#include "InputMouseKey.h"

#include "InputManager.h"

#include <AGUI/System.h>

namespace Atmos
{
    namespace Input
    {
        MouseKey::MouseKey(MouseKey&& arg) : Signal(std::move(arg))
        {}

        MouseKey::MouseKey(
            Manager& owner,
            ObjectManager& objectManager,
            DataPtr&& data,
            MouseKeyID id,
            Agui::MouseKeyID guiID,
            const String& displayName) :

            Signal(owner, objectManager, std::move(data), id, guiID, displayName, false)
        {}

        void MouseKey::DoActiveImpl()
        {
            auto& mousePositionDelta = Owner()->GetMousePositionDelta();
            Owner()->eventMouseKeys.active(*this);
            if (mousePositionDelta.Moved())
                Owner()->eventMouseClickDragged(mousePositionDelta);
        }

        void MouseKey::DoUpImpl()
        {
            Owner()->eventMouseKeys.pressed(*this);
            ::Agui::System::InjectMouseButtonUp(guiId);
        }

        void MouseKey::DoDownImpl()
        {
            Owner()->eventMouseKeys.depressed(*this);
            ::Agui::System::InjectMouseButtonDown(guiId);
        }

        void MouseKey::DoDoubleDownImpl()
        {
            Owner()->eventMouseKeys.doublePressed(*this);
        }

        bool MouseKey::IsKey() const
        {
            return false;
        }

        bool MouseKey::IsMouseKey() const
        {
            return true;
        }
    }
}