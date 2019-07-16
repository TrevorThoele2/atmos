
#include "InputSignalBase.h"

#include "ObjectManager.h"

namespace Atmos::Input
{
    void SignalBase::Work(bool currentValue)
    {
        activePreviousFrame = active;
        if (currentValue)
        {
            active = true;
            DoActive();
        }
        else
            active = false;

        if (IsPressed())
            DoDown();
        else if (IsDepressed())
            DoUp();
    }

    bool SignalBase::IsActive() const
    {
        return active;
    }

    bool SignalBase::IsPressed() const
    {
        return !activePreviousFrame && active;
    }

    bool SignalBase::IsDepressed() const
    {
        return activePreviousFrame && !active;
    }

    bool SignalBase::CanUseForAction() const
    {
        return canUseForAction;
    }

    SignalBase::Data::~Data()
    {}

    SignalBase::Data* SignalBase::GetData()
    {
        return data.get();
    }

    const SignalBase::Data* SignalBase::GetData() const
    {
        return data.get();
    }

    SignalBase::SignalBase(Manager& owner, ObjectManager& objectManager, DataPtr&& data, const String& displayName, bool canUseForAction) :
        owner(&owner), objectManager(&objectManager), displayName(displayName), canUseForAction(canUseForAction),
        active(false), activePreviousFrame(false),
        doubleClickStopwatch(
            objectManager.CreateObject<FrameStopwatch>(
                TimeValue(FixedPoint64::Split(0, FixedPoint64::Split::AdjustF(25, FixedPoint64::GetDefaultRadixPoint())))))
    {
        SetData(std::move(data));
    }

    SignalBase::SignalBase(SignalBase&& arg) :
        owner(std::move(arg.owner)), objectManager(std::move(arg.objectManager)),
        displayName(std::move(arg.displayName)), canUseForAction(std::move(arg.canUseForAction)),
        active(std::move(arg.active)), activePreviousFrame(std::move(arg.activePreviousFrame)),
        doubleClickStopwatch(std::move(arg.doubleClickStopwatch))
    {}

    Manager* SignalBase::Owner()
    {
        return owner;
    }

    const Manager* SignalBase::Owner() const
    {
        return owner;
    }

    ObjectManager* SignalBase::GetObjectManager()
    {
        return objectManager;
    }

    const ObjectManager* SignalBase::GetObjectManager() const
    {
        return objectManager;
    }

    void SignalBase::DoActive()
    {
        DoActiveImpl();
    }

    void SignalBase::DoUp()
    {
        DoUpImpl();
    }

    void SignalBase::DoDown()
    {
        DoDownImpl();

        // Check for double click
        if (!doubleClickStopwatch->HasReachedGoal())
            DoDoubleDownImpl();

        // Reset the double click timer
        doubleClickStopwatch->Start();
    }

    void SignalBase::SetData(DataPtr&& set)
    {
        data = std::move(set);
        data->owner = this;
    }
}