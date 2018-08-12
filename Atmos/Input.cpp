
#include "Input.h"
#include "Environment.h"

#include <AGUI/System.h>

namespace Atmos
{
    namespace Input
    {
        void SignalBase::Active()
        {
            ActiveImpl();
        }

        void SignalBase::Up()
        {
            UpImpl();
        }

        void SignalBase::Down()
        {
            DownImpl();

            // Check for double click
            if (!doubleClickTimer.HasReachedGoal())
                DoubleDownImpl();

            // Reset the double click timer
            doubleClickTimer.Start();
        }

        void SignalBase::SetData(Data *set)
        {
            data.reset(set);
            data->owner = this;
        }

        void SignalBase::SetData(std::unique_ptr<Data> &&set)
        {
            data = std::move(set);
            data->owner = this;
        }

        SignalBase::SignalBase(Data *data, const String &displayName, bool canUseForAction) : displayName(displayName), canUseForAction(canUseForAction), active(false), previousFrame(false), doubleClickTimer(TimeValue(FixedPoint64(0, 25)))
        {
            SetData(data);
        }

        SignalBase::Data* SignalBase::GetData()
        {
            return data.get();
        }

        const SignalBase::Data* SignalBase::GetData() const
        {
            return data.get();
        }

        void SignalBase::Work(bool curValue)
        {
            previousFrame = active;
            if (curValue)
            {
                active = true;
                Active();
            }
            else
                active = false;

            if (Pressed())
                Down();
            else if (Depressed())
                Up();
        }

        bool SignalBase::IsActive() const
        {
            return active;
        }

        bool SignalBase::Pressed() const
        {
            return !previousFrame && active;
        }

        bool SignalBase::Depressed() const
        {
            return previousFrame && !active;
        }

        bool SignalBase::CanUseForAction() const
        {
            return canUseForAction;
        }

        Key* SignalBase::AsKey()
        {
            if (!IsKey())
                return nullptr;

            return static_cast<Key*>(this);
        }

        const Key* SignalBase::AsKey() const
        {
            if (!IsKey())
                return nullptr;

            return static_cast<const Key*>(this);
        }

        MouseKey* SignalBase::AsMouseKey()
        {
            if (!IsMouseKey())
                return nullptr;

            return static_cast<MouseKey*>(this);
        }

        const MouseKey* SignalBase::AsMouseKey() const
        {
            if (!IsMouseKey())
                return nullptr;

            return static_cast<const MouseKey*>(this);
        }

        void Key::ActiveImpl()
        {
            Environment::GetInput()->eventKeys.active(*this);
            Agui::System::InjectInputActive(guiId);
        }

        void Key::UpImpl()
        {
            Environment::GetInput()->eventKeys.pressed(*this);
            Agui::System::InjectInputUp(guiId);
        }

        void Key::DownImpl()
        {
            Environment::GetInput()->eventKeys.depressed(*this);
            Agui::System::InjectInputDown(guiId);
        }

        void Key::DoubleDownImpl()
        {
            Environment::GetInput()->eventKeys.doublePressed(*this);
        }

        Key::Key(Data *data, KeyID id, Agui::Key guiID, const String &displayName, bool canUseForAction) : Signal(data, id, guiID, displayName, canUseForAction)
        {}

        bool Key::IsKey() const
        {
            return true;
        }

        bool Key::IsMouseKey() const
        {
            return false;
        }

        void MouseKey::ActiveImpl()
        {
            auto &mousePosDelta = Environment::GetInput()->GetMousePositionDelta();
            Environment::GetInput()->eventMouseKeys.active(*this);
            if (mousePosDelta.Moved())
                Environment::GetInput()->eventMouseClickDragged(mousePosDelta);
        }

        void MouseKey::UpImpl()
        {
            Environment::GetInput()->eventMouseKeys.pressed(*this);
            Agui::System::InjectMouseButtonUp(guiId);
        }

        void MouseKey::DownImpl()
        {
            Environment::GetInput()->eventMouseKeys.depressed(*this);
            Agui::System::InjectMouseButtonDown(guiId);
        }

        void MouseKey::DoubleDownImpl()
        {
            Environment::GetInput()->eventMouseKeys.doublePressed(*this);
        }

        MouseKey::MouseKey(Data *data, MouseButton id, Agui::MouseButton guiID, const String &displayName) : Signal(data, id, guiID, displayName, false)
        {}

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