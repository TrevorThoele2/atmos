#pragma once

#include <vector>
#include <map>
#include <unordered_map>

#include "Input.h"
#include "InputAction.h"

#include "Event.h"
#include "MousePosition.h"

namespace Atmos
{
    namespace Input
    {
        class KeyVariant
        {
        private:
            enum class Type
            {
                NONE,
                KEY,
                MOUSE_KEY
            };

            union U
            {
                KeyID keyID;
                MouseButton mouseKeyID;
                U() = default;
                U(KeyID keyID);
                U(MouseButton mouseKeyID);
            };
        private:
            Type t;
            U u;
        public:
            KeyVariant();
            KeyVariant(KeyID keyID);
            KeyVariant(MouseButton mouseKeyID);
            bool operator==(const KeyVariant &arg) const;
            bool operator!=(const KeyVariant &arg) const;
            void Set(KeyID keyID);
            void Set(MouseButton mouseKeyID);

            bool IsValid() const;
            bool IsKey() const;
            bool IsMouseKey() const;
            KeyID AsKey() const;
            MouseButton AsMouseKey() const;
        };

        class HandlerBase
        {
        public:
            typedef std::vector<Action*> ActionVector;
        private:
            template<class T>
            struct ButtonEvents
            {
                Event<const T&> active;
                Event<const T&> pressed;
                Event<const T&> doublePressed;
                Event<const T&> depressed;
            };
        protected:
            typedef std::vector<SignalBase*> InputVector;
        private:
            InputVector inputVector;
            std::unordered_map<KeyID, Key> keyInputMap;
            std::unordered_map<MouseButton, MouseKey> mouseKeyInputMap;

            std::map<ActionID, Action> actionMap;
            ActionVector actionVector;

            MousePosition mousePos, mousePosPrev, mousePosPress;
            MousePositionDelta mousePosDelta;

            HandlerBase(const HandlerBase &arg) = delete;
            HandlerBase& operator=(const HandlerBase &arg) = delete;

            virtual void AddInputsImpl() = 0;
            virtual void WorkInputsImpl(InputVector &inputs) = 0;
            virtual bool ShouldAddActions() const = 0;
            virtual MousePosition GetMousePositionImpl() const = 0;

            void OnIniLoad();

            void SetMousePos();
            void SetMousePosPress();
            void ResetMousePosPress();

            template<ActionID id>
            void AddAction(SignalBase &defaultKey);
        protected:
            HandlerBase() = default;

            template<KeyID id>
            Key& AddInput(SignalBase::Data *data);
            template<MouseButton id>
            MouseKey& AddMouseInput(SignalBase::Data *data);
        public:
            ButtonEvents<Key> eventKeys;
            ButtonEvents<MouseKey> eventMouseKeys;

            Event<const MousePositionDelta&> eventMouseMoved;
            Event<const MousePositionDelta&> eventMouseClickDragged;

            Event<const Action&> eventActionActive;
            Event<const Action&> eventActionPressed;
            Event<const Action&> eventActionDepressed;

            virtual ~HandlerBase() = 0 {}

            void InitCreateInputs();
            void InitCreateActions();
            void InitFinalize();
            void Work();

            bool IsInputActive(KeyID id);
            bool IsInputInactive(KeyID id);
            bool IsInputPressed(KeyID id);
            bool IsInputDepressed(KeyID id);
            bool IsInputActive(MouseButton id);
            bool IsInputInactive(MouseButton id);
            bool IsInputPressed(MouseButton id);
            bool IsInputDepressed(MouseButton id);
            bool IsActionActive(ActionID id);
            bool IsActionInactive(ActionID id);
            bool IsActionPressed(ActionID id);
            bool IsActionDepressed(ActionID id);

            bool IsKeyUsedForAction(const SignalBase &input);

            Key* GetKey(KeyID id);
            MouseKey* GetMouseKey(MouseButton id);
            Action* GetAction(ActionID id);
            SignalBase* GetInputFromDisplayName(const String &displayName);
            Action* GetActionFromDisplayName(const String &displayName);

            MousePosition GetMousePosition();
            MousePosition GetMousePositionAtPress();
            MousePosition GetMousePositionInGameCoordinates();
            MousePositionDelta GetMousePositionDelta();
            ActionVector& GetActions();
        };

        template<ActionID id>
        void HandlerBase::AddAction(SignalBase &defaultKey)
        {
            actionVector.push_back(&actionMap.emplace(id, Action(id, ActionTraits<id>::displayName, ActionTraits<id>::iniID, defaultKey)).first->second);
        }

        template<KeyID id>
        Key& HandlerBase::AddInput(SignalBase::Data *data)
        {
            auto &emplaced = keyInputMap.emplace(id, Key(data, id, KeyTraits<id>::aguiID, KeyTraits<id>::displayName, KeyTraits<id>::canUseForAction)).first->second;
            inputVector.push_back(&emplaced);
            return emplaced;
        }

        template<MouseButton id>
        MouseKey& HandlerBase::AddMouseInput(SignalBase::Data *data)
        {
            auto &emplaced = mouseKeyInputMap.emplace(id, MouseKey(data, id, MouseTraits<id>::aguiID, MouseTraits<id>::displayName)).first->second;
            inputVector.push_back(&emplaced);
            return emplaced;
        }
    }
}