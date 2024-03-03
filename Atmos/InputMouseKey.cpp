
#include "InputMouseKey.h"

#include "InputManager.h"

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
            ::Agui::Input::Signal* guiSignal,
            const String& displayName) :

            Signal(owner, objectManager, std::move(data), id, guiSignal, displayName, false)
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
        }

        void MouseKey::DoDownImpl()
        {
            Owner()->eventMouseKeys.depressed(*this);
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