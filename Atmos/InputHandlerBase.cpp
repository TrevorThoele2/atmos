
#include "InputHandlerBase.h"

#include "Environment.h"
#include "Camera.h"
#include "Debug.h"

#include <AGUI/System.h>

namespace Atmos
{
    namespace Input
    {
        KeyVariant::U::U(KeyID keyID) : keyID(keyID)
        {}

        KeyVariant::U::U(MouseButton mouseKeyID) : mouseKeyID(mouseKeyID)
        {}

        KeyVariant::KeyVariant() : t(Type::NONE)
        {}

        KeyVariant::KeyVariant(KeyID keyID) : t(Type::KEY), u(keyID)
        {}

        KeyVariant::KeyVariant(MouseButton mouseKeyID) : t(Type::MOUSE_KEY), u(mouseKeyID)
        {}

        bool KeyVariant::operator==(const KeyVariant &arg) const
        {
            if (t != arg.t)
                return false;

            if (t == Type::NONE)
                return true;

            if (t == Type::KEY)
                return u.keyID == arg.u.keyID;
            else
                return u.mouseKeyID == arg.u.mouseKeyID;
        }

        bool KeyVariant::operator!=(const KeyVariant &arg) const
        {
            return !(*this == arg);
        }

        void KeyVariant::Set(KeyID keyID)
        {
            t = Type::KEY;
            u = U(keyID);
        }

        void KeyVariant::Set(MouseButton mouseKeyID)
        {
            t = Type::MOUSE_KEY;
            u = U(mouseKeyID);
        }

        bool KeyVariant::IsValid() const
        {
            return t != Type::NONE;
        }

        bool KeyVariant::IsKey() const
        {
            return t == Type::KEY;
        }

        bool KeyVariant::IsMouseKey() const
        {
            return t == Type::MOUSE_KEY;
        }

        KeyID KeyVariant::AsKey() const
        {
            return u.keyID;
        }

        MouseButton KeyVariant::AsMouseKey() const
        {
            return u.mouseKeyID;
        }

        void HandlerBase::OnIniLoad()
        {
            for (auto &actionLoop : actionMap)
                actionLoop.second.SetMappedKey(static_cast<Ini::Manager::InputEntry*>(Environment::GetIni().GetEntry(actionLoop.second.iniID))->value);
        }

        void HandlerBase::SetMousePos()
        {
            mousePos = GetMousePositionImpl();
        }

        void HandlerBase::SetMousePosPress()
        {
            mousePosPress = mousePos;
        }

        void HandlerBase::ResetMousePosPress()
        {
            mousePosPress.Set(0, 0);
        }

        void HandlerBase::InitCreateInputs()
        {
            AddInputsImpl();
        }

        void HandlerBase::InitCreateActions()
        {
            AddAction<ActionID::MOVE_LEFT>(keyInputMap.find(*Environment::GetIni().GetEntry<Ini::ID::MOVE_LEFT>())->second);
            AddAction<ActionID::MOVE_UP>(keyInputMap.find(*Environment::GetIni().GetEntry<Ini::ID::MOVE_UP>())->second);
            AddAction<ActionID::MOVE_RIGHT>(keyInputMap.find(*Environment::GetIni().GetEntry<Ini::ID::MOVE_RIGHT>())->second);
            AddAction<ActionID::MOVE_DOWN>(keyInputMap.find(*Environment::GetIni().GetEntry<Ini::ID::MOVE_DOWN>())->second);
            AddAction<ActionID::USE>(keyInputMap.find(*Environment::GetIni().GetEntry<Ini::ID::USE>())->second);
            AddAction<ActionID::NAVIGATE_MENU_LEFT>(keyInputMap.find(*Environment::GetIni().GetEntry<Ini::ID::MENU_LEFT>())->second);
            AddAction<ActionID::NAVIGATE_MENU_UP>(keyInputMap.find(*Environment::GetIni().GetEntry<Ini::ID::MENU_UP>())->second);
            AddAction<ActionID::NAVIGATE_MENU_RIGHT>(keyInputMap.find(*Environment::GetIni().GetEntry<Ini::ID::MENU_RIGHT>())->second);
            AddAction<ActionID::NAVIGATE_MENU_DOWN>(keyInputMap.find(*Environment::GetIni().GetEntry<Ini::ID::MENU_DOWN>())->second);
            AddAction<ActionID::CHANGE_SELECTED_CHARACTER_LEFT>(keyInputMap.find(*Environment::GetIni().GetEntry<Ini::ID::SELECT_CHARACTER_LEFT>())->second);
            AddAction<ActionID::CHANGE_SELECTED_CHARACTER_RIGHT>(keyInputMap.find(*Environment::GetIni().GetEntry<Ini::ID::SELECT_CHARACTER_RIGHT>())->second);
            AddAction<ActionID::INVENTORY>(keyInputMap.find(*Environment::GetIni().GetEntry<Ini::ID::INVENTORY>())->second);
            AddAction<ActionID::END_TURN>(keyInputMap.find(*Environment::GetIni().GetEntry<Ini::ID::END_TURN>())->second);
            AddAction<ActionID::ATTACK>(keyInputMap.find(*Environment::GetIni().GetEntry<Ini::ID::ATTACK>())->second);
            AddAction<ActionID::OPEN_SPELLS>(keyInputMap.find(*Environment::GetIni().GetEntry<Ini::ID::SPELLS>())->second);
            AddAction<ActionID::CANCEL>(keyInputMap.find(*Environment::GetIni().GetEntry<Ini::ID::CANCEL>())->second);
            AddAction<ActionID::STATS>(keyInputMap.find(*Environment::GetIni().GetEntry<Ini::ID::STATS>())->second);
        }

        void HandlerBase::InitFinalize()
        {
            Environment::GetIni().eventPostLoad.Subscribe(&HandlerBase::OnIniLoad, *this);

            ResetMousePosPress();
            SetMousePos();
            mousePosDelta.Setup(mousePos);
        }

        void HandlerBase::Work()
        {
            DebugScreen::Input().Start();

            // Mouse input
            mousePosPrev = mousePos;
            SetMousePos();
            mousePosDelta.Update(mousePos);
            Agui::System::InjectMousePosition(static_cast<Agui::Position::ValueT>(mousePos.x), static_cast<Agui::Position::ValueT>(mousePos.y));

            // Work inputs
            WorkInputsImpl(inputVector);

            // Actions
            for (auto &loop : actionMap)
                loop.second.Work();

            DebugScreen::Input().Calc();
        }

        bool HandlerBase::IsInputActive(KeyID id)
        {
            return keyInputMap.find(id)->second.IsActive();
        }

        bool HandlerBase::IsInputInactive(KeyID id)
        {
            return !IsInputActive(id);
        }

        bool HandlerBase::IsInputActive(MouseButton id)
        {
            return mouseKeyInputMap.find(id)->second.IsActive();
        }

        bool HandlerBase::IsInputInactive(MouseButton id)
        {
            return !IsInputActive(id);
        }

        bool HandlerBase::IsActionActive(ActionID id)
        {
            return actionMap.find(id)->second.mappedKey->IsActive();
        }

        bool HandlerBase::IsActionInactive(ActionID id)
        {
            return !IsActionActive(id);
        }

        bool HandlerBase::IsActionPressed(ActionID id)
        {
            return actionMap.find(id)->second.mappedKey->Pressed();
        }

        bool HandlerBase::IsActionDepressed(ActionID id)
        {
            return actionMap.find(id)->second.mappedKey->Depressed();
        }

        bool HandlerBase::IsKeyUsedForAction(const SignalBase &input)
        {
            for (auto &loop : actionMap)
            {
                if (loop.second.mappedKey == &input)
                    return true;
            }

            return false;
        }

        Key* HandlerBase::GetKey(KeyID id)
        {
            auto found = keyInputMap.find(id);
            if (found == keyInputMap.end())
                return nullptr;

            return &found->second;
        }

        MouseKey* HandlerBase::GetMouseKey(MouseButton id)
        {
            auto found = mouseKeyInputMap.find(id);
            if (found == mouseKeyInputMap.end())
                return nullptr;

            return &found->second;
        }

        Action* HandlerBase::GetAction(ActionID id)
        {
            auto found = actionMap.find(id);
            if (found == actionMap.end())
                return nullptr;

            return &found->second;
        }

        SignalBase* HandlerBase::GetInputFromDisplayName(const String &displayName)
        {
            for (auto &loop : inputVector)
            {
                if (loop->displayName == displayName)
                    return loop;
            }

            return nullptr;
        }

        Action* HandlerBase::GetActionFromDisplayName(const String &displayName)
        {
            for (auto &loop : actionMap)
            {
                if (loop.second.displayName == displayName)
                    return &loop.second;
            }

            return nullptr;
        }

        MousePosition HandlerBase::GetMousePosition()
        {
            return mousePos;
        }

        MousePosition HandlerBase::GetMousePositionAtPress()
        {
            return mousePosPress;
        }

        MousePosition HandlerBase::GetMousePositionInGameCoords()
        {
            MousePosition toReturn;

            toReturn.x = static_cast<MousePosition::ValueT>(floor(static_cast<double>(mousePos.x) + Camera::GetTopLeft().GetX()));
            toReturn.y = static_cast<MousePosition::ValueT>(floor(static_cast<double>(mousePos.y) + Camera::GetTopLeft().GetY()));

            return toReturn;
        }

        MousePositionDelta HandlerBase::GetMousePositionDelta()
        {
            return MousePositionDelta(mousePos, mousePosPrev);
        }

        HandlerBase::ActionVector& HandlerBase::GetActions()
        {
            return actionVector;
        }
    }
}