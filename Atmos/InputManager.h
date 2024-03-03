#pragma once

#include <vector>
#include <map>
#include <unordered_map>

#include "InputKey.h"
#include "InputMouseKey.h"
#include "InputAction.h"
#include "InputTraits.h"

#include "Event.h"
#include "MousePosition.h"

namespace Atmos
{
    class ObjectManager;
    class CameraSystem;
    class DebugStatisticsSystem;

    namespace Input
    {
        class Manager
        {
        public:
            typedef std::vector<Action*> ActionList;
        public:
            template<class T>
            struct ButtonEvents
            {
                Event<T> active;
                Event<T> pressed;
                Event<T> doublePressed;
                Event<T> depressed;
            };
        public:
            ButtonEvents<const Key&> eventKeys;
            ButtonEvents<const MouseKey&> eventMouseKeys;

            Event<const MousePositionDelta&> eventMouseMoved;
            Event<const MousePositionDelta&> eventMouseClickDragged;

            Event<const Action&> eventActionActive;
            Event<const Action&> eventActionPressed;
            Event<const Action&> eventActionDepressed;
        public:
            Manager(ObjectManager& objectManager);
            virtual ~Manager() = 0;

            void Initialize();
            void Work();

            bool IsSignalActive(KeyID id);
            bool IsSignalPressed(KeyID id);
            bool IsSignalDepressed(KeyID id);
            bool IsSignalActive(MouseKeyID id);
            bool IsSignalPressed(MouseKeyID id);
            bool IsSignalDepressed(MouseKeyID id);
            bool IsActionActive(ActionID id);
            bool IsActionPressed(ActionID id);
            bool IsActionDepressed(ActionID id);

            bool IsKeyUsedForAction(const SignalBase& input);

            Key* GetKey(KeyID id);
            MouseKey* GetMouseKey(MouseKeyID id);
            Action* GetAction(ActionID id);
            SignalBase* GetSignalFromDisplayName(const String& displayName);
            Action* GetActionFromDisplayName(const String& displayName);

            MousePosition GetMousePosition();
            MousePosition GetMousePositionAtPress();
            MousePosition GetMousePositionInGameCoordinates();
            MousePositionDelta GetMousePositionDelta();
            ActionList& GetActions();
        protected:
            typedef std::vector<SignalBase*> SignalList;
            typedef SignalBase::DataPtr SignalDataPtr;
        protected:
            Manager();

            template<KeyID id>
            Key* CreateKey(SignalDataPtr&& data);
            template<MouseKeyID id>
            MouseKey* CreateMouseKey(SignalDataPtr&& data);
        protected:
            ObjectManager* GetObjectManager() const;
            CameraSystem* FindCameraSystem() const;
            DebugStatisticsSystem* FindDebugStatisticsSystem() const;
        private:
            SignalList signalList;
            std::unordered_map<KeyID, Key> keyMap;
            std::unordered_map<MouseKeyID, MouseKey> mouseKeyMap;
        private:
            std::map<ActionID, Action> actionMap;
            ActionList actionList;
        private:
            MousePosition mousePosition;
            MousePosition previousMousePosition;
            MousePosition mousePositionAtPress;
            MousePositionDelta mousePositionDelta;
        private:
            void CreateAllSignals();
            void CreateAllActions();
        private:
            void SetMousePosition();
            void SetMousePositionPress();
            void ResetMousePositionPress();
        private:
            template<ActionID id>
            void CreateAction();
        private:
            virtual void CreateSignalsImpl() = 0;
            virtual void WorkSignalsImpl(SignalList& signalList) = 0;
            virtual bool ShouldAddActions() const = 0;
            virtual MousePosition GetMousePositionImpl() const = 0;
        private:
            ObjectManager* objectManager;
        };

        template<KeyID id>
        Key* Manager::CreateKey(SignalDataPtr&& data)
        {
            Key key(*this, *objectManager, std::move(data), id, nullptr, KeyTraits<id>::displayName, KeyTraits<id>::canUseForAction);
            auto emplaced = &keyMap.emplace(id, std::move(key)).first->second;
            signalList.push_back(emplaced);
            return emplaced;
        }

        template<MouseKeyID id>
        MouseKey* Manager::CreateMouseKey(SignalDataPtr&& data)
        {
            MouseKey mouseKey(*this, *objectManager, std::move(data), id, nullptr, MouseTraits<id>::displayName);
            auto emplaced = &mouseKeyMap.emplace(id, std::move(mouseKey)).first->second;
            signalList.push_back(emplaced);
            return emplaced;
        }

        template<ActionID id>
        void Manager::CreateAction()
        {
            Action action(*this, id, ActionTraits<id>::displayName);
            auto& emplaced = actionMap.emplace(id, action).first->second;
            actionList.push_back(&emplaced);
        }
    }
}