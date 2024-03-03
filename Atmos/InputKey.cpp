
#include "InputKey.h"

#include "InputManager.h"

namespace Atmos::Input
{
    Key::Key(Key&& arg) : Signal(std::move(arg))
    {}

    Key::Key(
        Manager& owner,
        ObjectManager& objectManager,
        DataPtr&& data,
        KeyID id,
        ::Agui::Input::Signal* guiSignal,
        const String& displayName,
        bool canUseForAction) :

        Signal(owner, objectManager, std::move(data), id, guiSignal, displayName, canUseForAction)
    {}

    void Key::DoActiveImpl()
    {
        Owner()->eventKeys.active(*this);
    }

    void Key::DoUpImpl()
    {
        Owner()->eventKeys.pressed(*this);
    }

    void Key::DoDownImpl()
    {
        Owner()->eventKeys.depressed(*this);
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