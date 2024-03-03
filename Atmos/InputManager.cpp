
#include "InputManager.h"

#include "ObjectManager.h"
#include "CameraSystem.h"
#include "DebugStatisticsSystem.h"

namespace Atmos
{
    namespace Input
    {
        Manager::Manager(ObjectManager& objectManager) : objectManager(&objectManager)
        {}

        Manager::~Manager()
        {}

        void Manager::Initialize()
        {
            CreateAllSignals();
            CreateAllActions();

            ResetMousePositionPress();
            SetMousePosition();
            mousePositionDelta.Setup(mousePosition);
        }

        void Manager::Work()
        {
            FindDebugStatisticsSystem()->inputProfiler.Start();

            previousMousePosition = mousePosition;
            SetMousePosition();
            mousePositionDelta.Update(mousePosition);

            WorkSignalsImpl(signalList);

            // Actions
            for (auto& loop : actionMap)
                loop.second.Work();

            FindDebugStatisticsSystem()->inputProfiler.Calculate();
        }

        bool Manager::IsSignalActive(KeyID id)
        {
            return keyMap.find(id)->second.IsActive();
        }

        bool Manager::IsSignalPressed(KeyID id)
        {
            return keyMap.find(id)->second.IsPressed();
        }

        bool Manager::IsSignalDepressed(KeyID id)
        {
            return keyMap.find(id)->second.IsDepressed();
        }

        bool Manager::IsSignalActive(MouseKeyID id)
        {
            return mouseKeyMap.find(id)->second.IsActive();
        }

        bool Manager::IsSignalPressed(MouseKeyID id)
        {
            return mouseKeyMap.find(id)->second.IsPressed();
        }

        bool Manager::IsSignalDepressed(MouseKeyID id)
        {
            return mouseKeyMap.find(id)->second.IsDepressed();
        }

        bool Manager::IsActionActive(ActionID id)
        {
            return actionMap.find(id)->second.MappedSignal()->IsActive();
        }

        bool Manager::IsActionPressed(ActionID id)
        {
            return actionMap.find(id)->second.MappedSignal()->IsPressed();
        }

        bool Manager::IsActionDepressed(ActionID id)
        {
            return actionMap.find(id)->second.MappedSignal()->IsDepressed();
        }

        bool Manager::IsKeyUsedForAction(const SignalBase& input)
        {
            for (auto& loop : actionMap)
            {
                if (loop.second.MappedSignal() == &input)
                    return true;
            }

            return false;
        }

        Key* Manager::GetKey(KeyID id)
        {
            auto found = keyMap.find(id);
            if (found == keyMap.end())
                return nullptr;

            return &found->second;
        }

        MouseKey* Manager::GetMouseKey(MouseKeyID id)
        {
            auto found = mouseKeyMap.find(id);
            if (found == mouseKeyMap.end())
                return nullptr;

            return &found->second;
        }

        Action* Manager::GetAction(ActionID id)
        {
            auto found = actionMap.find(id);
            if (found == actionMap.end())
                return nullptr;

            return &found->second;
        }

        SignalBase* Manager::GetSignalFromDisplayName(const String& displayName)
        {
            for (auto& loop : signalList)
            {
                if (loop->displayName == displayName)
                    return loop;
            }

            return nullptr;
        }

        Action* Manager::GetActionFromDisplayName(const String& displayName)
        {
            for (auto& loop : actionMap)
            {
                if (loop.second.displayName == displayName)
                    return &loop.second;
            }

            return nullptr;
        }

        MousePosition Manager::GetMousePosition()
        {
            return mousePosition;
        }

        MousePosition Manager::GetMousePositionAtPress()
        {
            return mousePositionAtPress;
        }

        MousePosition Manager::GetMousePositionInGameCoordinates()
        {
            MousePosition toReturn;

            auto& cameraTopLeft = FindCameraSystem()->GetTopLeft();
            toReturn.x = static_cast<MousePosition::Value>(floor(static_cast<double>(mousePosition.x) + cameraTopLeft.x));
            toReturn.y = static_cast<MousePosition::Value>(floor(static_cast<double>(mousePosition.y) + cameraTopLeft.y));

            return toReturn;
        }

        MousePositionDelta Manager::GetMousePositionDelta()
        {
            return MousePositionDelta(mousePosition, previousMousePosition);
        }

        Manager::ActionList& Manager::GetActions()
        {
            return actionList;
        }

        Manager::Manager()
        {}

        ObjectManager* Manager::GetObjectManager() const
        {
            return objectManager;
        }

        CameraSystem* Manager::FindCameraSystem() const
        {
            return GetObjectManager()->FindSystem<CameraSystem>();
        }

        DebugStatisticsSystem* Manager::FindDebugStatisticsSystem() const
        {
            return GetObjectManager()->FindSystem<DebugStatisticsSystem>();
        }

        void Manager::CreateAllSignals()
        {
            CreateSignalsImpl();
        }

        void Manager::CreateAllActions()
        {
            CreateAction<ActionID::MOVE_LEFT>();
            CreateAction<ActionID::MOVE_UP>();
            CreateAction<ActionID::MOVE_RIGHT>();
            CreateAction<ActionID::MOVE_DOWN>();
            CreateAction<ActionID::USE>();
            CreateAction<ActionID::NAVIGATE_MENU_LEFT>();
            CreateAction<ActionID::NAVIGATE_MENU_UP>();
            CreateAction<ActionID::NAVIGATE_MENU_RIGHT>();
            CreateAction<ActionID::NAVIGATE_MENU_DOWN>();
            CreateAction<ActionID::CHANGE_SELECTED_CHARACTER_LEFT>();
            CreateAction<ActionID::CHANGE_SELECTED_CHARACTER_RIGHT>();
            CreateAction<ActionID::INVENTORY>();
            CreateAction<ActionID::END_TURN>();
            CreateAction<ActionID::ATTACK>();
            CreateAction<ActionID::OPEN_SPELLS>();
            CreateAction<ActionID::CANCEL>();
            CreateAction<ActionID::STATS>();
        }

        void Manager::SetMousePosition()
        {
            mousePosition = GetMousePositionImpl();
        }

        void Manager::SetMousePositionPress()
        {
            mousePositionAtPress = mousePosition;
        }

        void Manager::ResetMousePositionPress()
        {
            mousePositionAtPress.Set(0, 0);
        }
    }
}