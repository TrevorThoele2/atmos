#pragma once

#include "FrameStopwatch.h"
#include "ObjectReference.h"

#include "String.h"

namespace Atmos::Input
{
    class Manager;

    class SignalBase
    {
    public:
        const String displayName;
    public:
        void Work(bool currentValue);

        bool IsActive() const;
        bool IsPressed() const;
        bool IsDepressed() const;
        bool CanUseForAction() const;
    public:
        class Data
        {
        public:
            virtual ~Data() = 0;
        private:
            SignalBase* owner;
        private:
            friend SignalBase;
        };
    public:
        template<class DerivedT>
        DerivedT* GetData();
        template<class DerivedT>
        const DerivedT* GetData() const;
        Data* GetData();
        const Data* GetData() const;
    protected:
        typedef std::unique_ptr<Data> DataPtr;
    protected:
        SignalBase(Manager& owner, ObjectManager& objectManager, DataPtr&& data, const String& displayName, bool canUseForAction);
        SignalBase(SignalBase&& arg);

        Manager* Owner();
        const Manager* Owner() const;

        ObjectManager* GetObjectManager();
        const ObjectManager* GetObjectManager() const;
    private:
        Manager* owner;

        ObjectManager* objectManager;
    private:
        bool active;
        bool activePreviousFrame;
        bool canUseForAction;
        typedef TypedObjectReference<FrameStopwatch> FrameStopwatchReference;
        FrameStopwatchReference doubleClickStopwatch;
    private:
        void DoActive();
        void DoUp();
        void DoDown();
    private:
        DataPtr data;
        void SetData(DataPtr&& set);
    private:
        virtual void DoActiveImpl() = 0;
        virtual void DoUpImpl() = 0;
        virtual void DoDownImpl() = 0;
        virtual void DoDoubleDownImpl() = 0;

        virtual bool IsKey() const = 0;
        virtual bool IsMouseKey() const = 0;
    };

    template<class DerivedT>
    DerivedT* SignalBase::GetData()
    {
        return static_cast<DerivedT*>(data.get());
    }

    template<class DerivedT>
    const DerivedT* SignalBase::GetData() const
    {
        return static_cast<DerivedT*>(data.get());
    }
}