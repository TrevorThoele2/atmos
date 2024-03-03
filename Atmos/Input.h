#pragma once

#include <memory>

#include "InputDefines.h"
#include "FrameTimer.h"
#include "String.h"

namespace Atmos
{
    namespace Input
    {
        class Key;
        class MouseKey;
        class SignalBase
        {
        public:
            class Data
            {
            private:
                SignalBase *owner;
                friend SignalBase;
            public:
                virtual ~Data() = 0 {}
            };
        private:
            bool active, previousFrame;
            FrameTimer doubleClickTimer;
            bool canUseForAction;
            std::unique_ptr<Data> data;

            void Active();
            void Up();
            void Down();
            virtual void ActiveImpl() = 0;
            virtual void UpImpl() = 0;
            virtual void DownImpl() = 0;
            virtual void DoubleDownImpl() = 0;

            void SetData(Data *set);
            void SetData(std::unique_ptr<Data> &&set);
        protected:
            SignalBase(Data *data, const String &displayName, bool canUseForAction);
        public:
            const String displayName;

            template<class DerivedT>
            DerivedT* GetData();
            template<class DerivedT>
            const DerivedT* GetData() const;
            Data* GetData();
            const Data* GetData() const;

            void Work(bool curValue);
            bool IsActive() const;
            bool IsPressed() const;
            bool IsDepressed() const;
            bool CanUseForAction() const;

            virtual bool IsKey() const = 0;
            virtual bool IsMouseKey() const = 0;
            Key* AsKey();
            const Key* AsKey() const;
            MouseKey* AsMouseKey();
            const MouseKey* AsMouseKey() const;
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

        template<class T, class GuiT>
        class Signal : public SignalBase
        {
        protected:
            Signal(Data *data, T id, GuiT guiId, const String &displayName, bool canUseForAction);
        public:
            T id;
            GuiT guiId;
        };

        template<class T, class GuiT>
        Signal<T, GuiT>::Signal(Data *data, T id, GuiT guiId, const String &displayName, bool canUseForAction) : SignalBase(data, displayName, canUseForAction), id(id), guiId(guiId)
        {}

        class HandlerBase;
        class Key : public Signal<KeyID, Agui::Key>
        {
        private:
            friend HandlerBase;
        private:
            void ActiveImpl() override;
            void UpImpl() override;
            void DownImpl() override;
            void DoubleDownImpl() override;

            Key(Data *data, KeyID id, Agui::Key guiID, const String &displayName, bool canUseForAction);
            bool IsKey() const override;
            bool IsMouseKey() const override;
        };

        class MouseKey : public Signal<MouseButton, Agui::MouseButton>
        {
        private:
            friend HandlerBase;
        private:
            void ActiveImpl() override;
            void UpImpl() override;
            void DownImpl() override;
            void DoubleDownImpl() override;

            MouseKey(Data *data, MouseButton id, Agui::MouseButton guiID, const String &displayName);
            bool IsKey() const override;
            bool IsMouseKey() const override;
        };
    }
}