#pragma once

#include "Random.h"
#include "InputDefines.h"
#include "TimeHandler.h"
#include "FrameTimer.h"
#include "Currency.h"
#include "Sprite.h"
#include "FieldID.h"
#include "Speech.h"
#include "Pathfinding.h"
#include "Name.h"
#include "OffsetManagedList.h"
#include "Script.h"
#include "ActionID.h"
#include "ActionParameter.h"
#include "ModulatorController.h"

#include "Entity.h"
#include "SenseComponent.h"
#include "InventoryComponent.h"
#include "BattleComponent.h"

#include "GridPosition.h"

namespace Atmos
{
    class Script;
    namespace Act
    {
        namespace Random
        {
            bool GenerateBool(double probability = 0.5);
            template<class T>
            T GenerateIntFullRange()
            {
                return ::Atmos::Random::Generate<T>();
            }

            template<class T>
            T GenerateInt(T floor, T ceiling)
            {
                return ::Atmos::Random::Generate(floor, ceiling);
            }
        }

        namespace Input
        {
            bool IsActionActive(::Atmos::Input::ActionID id);
            bool IsActionPressed(::Atmos::Input::ActionID id);
            bool IsActionDepressed(::Atmos::Input::ActionID id);
        }

        namespace Time
        {
            TimeValue GetFrameTime();
        }

        namespace Speech
        {
            void SetCharacters(const String &str);
            void AppendCharacters(const String &str);
            void ClearCharacters();
            const String& GetOutputCharacters();
            
            void StartSpeech(const Name &name);
            void LeaveSpeech();
            void EnterShop(bool buying);
            void LeaveShop();
            bool IsShopActive();
            Entity GetEntityTalking();

            void ActivateInput(std::vector<String> &&strings);
            void DeactivateInput();
            ::Atmos::Speech::Handler::InputID GetInputPosition();
        }

        namespace Script
        {
            void CreateAndStart(const Name &name);
            void CreateAndStartGlobal(const Name &name, const std::vector<Variant> &globalNames, const std::vector<Variant> &globalValues);
            void CreateAndStartExternal(const Name &name, const ::Atmos::Script::SymbolName &overrideExecuteName, const std::vector<Variant> &overrideParameterNames, const std::vector<Variant> &overrideParameterValues);
            void CreateAndStartGlobalExternal(const Name &name, const std::vector<Variant> &globalNames, const std::vector<Variant> &globalValues, const ::Atmos::Script::SymbolName &overrideExecuteName, const std::vector<Variant> &overrideParameterNames, const std::vector<Variant> &overrideParameterValues);
        }

        namespace Ent
        {
            void Move(Entity entity, const GridPosition &moveTo, TimeValue timeTaken);
            void Move(Entity entity, const Direction &direction);

            void SetSpriteVisibility(Entity entity, OffsetManagedList<Sprite>::ID spriteID, bool set);
            void EnableSprite(Entity entity, OffsetManagedList<Sprite>::ID spriteID);
            void DisableSprite(Entity entity, OffsetManagedList<Sprite>::ID spriteID);

            ::Atmos::Ent::InventoryComponent::Container::size_type GetItemCount(Entity entity);
            bool IsEntityInParty(Entity entity);

            Gold GetAvatarGold();
            void AddToPlayerParty(Entity entity);

            void AttachModulatorToSense(Entity entity, ::Atmos::Modulator::Controller::ID modID);
        }

        namespace Position
        {
            ::Atmos::Pathfinder::TileStack FindPath(const GridPosition &from, const GridPosition &to);
        }

        namespace Battle
        {
            void GivePlacement(const std::vector<::Atmos::Variant> &give);
            Entity FindClosestPlayer(const GridPosition &focus);
            Entity FindClosestMonster(const GridPosition &focus);
            void EndTurn(Entity entity);
        }

        namespace Camera
        {
            void Move(const Direction &dir, float distance);
            void MoveTo(const Position2D &pos, const FrameTimer &timer);
            void MoveToInstant(const Position2D &pos);
            void MoveDeltaInstant(const Position2D &delta);
            void MoveToFromScreenPos(const Position2D &pos, const FrameTimer &timer);
        }

        namespace Field
        {
            void Change(const Direction &dir, const GridPosition &pos, FieldID id);
        }

        namespace Music
        {
            void Change(const FileName &name);
        }

        namespace Scheduler
        {
            void Schedule(::Atmos::Act::ID id, const std::vector<::Atmos::Variant> &args, TimeValue::ValueT timeTaken);
        }
    }
}