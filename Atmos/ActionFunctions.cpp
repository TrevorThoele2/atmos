
#include "ActionFunctions.h"

#include "Input.h"
#include "Speech.h"
#include "Camera.h"
#include "WorldManager.h"
#include "FieldDestination.h"
#include "Battle.h"
#include "Music.h"
#include "CurrentMusic.h"
#include "Scheduler.h"
#include "ScriptRegistry.h"
#include "ScriptController.h"
#include "Action.h"

#include "EntityManager.h"
#include "AvatarSystem.h"
#include "AvatarComponent.h"
#include "DialogueComponent.h"
#include "EntityNameSystem.h"
#include "EntityPositionSystem.h"
#include "EntityAISystem.h"

#include "PlayerParty.h"
#include "Environment.h"

#include <Affecter/SmoothstepType.h>

namespace Atmos
{
    namespace Act
    {
        namespace Random
        {
            bool GenerateBool(double probability)
            {
                return ::Atmos::Random::GenerateBool(probability);
            }
        }

        namespace Input
        {
            bool IsActionActive(::Atmos::Input::ActionID id)
            {
                return Environment::GetInput()->IsActionActive(id);
            }

            bool IsActionPressed(::Atmos::Input::ActionID id)
            {
                return Environment::GetInput()->IsActionPressed(id);
            }

            bool IsActionDepressed(::Atmos::Input::ActionID id)
            {
                return Environment::GetInput()->IsActionDepressed(id);
            }
        }

        namespace Time
        {
            TimeValue GetFrameTime()
            {
                return Environment::GetTime().GetTotalElapsed();
            }
        }

        namespace Speech
        {
            void SetCharacters(const String &str)
            {
                ::Atmos::Speech::Handler::SetString(str);
            }

            void AppendCharacters(const String &str)
            {
                ::Atmos::Speech::Handler::AppendString(str);
            }

            void ClearCharacters()
            {
                ::Atmos::Speech::Handler::ClearString();
            }

            const String& GetOutputCharacters()
            {
                return ::Atmos::Speech::Handler::GetOutput();
            }

            void StartSpeech(const Name &name)
            {
                auto entity = ::Atmos::Ent::NameSystem::FindEntity(name);
                if (entity == ::Atmos::Ent::nullEntity)
                    return;

                auto dialogue = GetCurrentEntities()->FindComponent<::Atmos::Ent::DialogueComponent>(entity);
                if (!dialogue)
                    return;

                ::Atmos::Speech::Handler::Enter(*dialogue);
            }

            void LeaveSpeech()
            {
                ::Atmos::Speech::Handler::Leave();
            }

            void EnterShop(bool buying)
            {
                ::Atmos::Speech::Shop::Enter(buying);
            }

            bool IsShopActive()
            {
                return ::Atmos::Speech::Shop::IsActive();
            }

            Entity GetEntityTalking()
            {
                return ::Atmos::Speech::Handler::GetEntityTalking();
            }

            void ActivateInput(std::vector<String> &&strings)
            {
                ::Atmos::Speech::Handler::ActivateInput(std::move(strings));
            }

            void DeactivateInput()
            {
                ::Atmos::Speech::Handler::DeactivateInput();
            }

            ::Atmos::Speech::Handler::InputID GetInputPosition()
            {
                return ::Atmos::Speech::Handler::GetInputPosition();
            }
        }

        namespace Script
        {
            void CreateAndStart(const Name &name)
            {
                auto found = AssetRegistry<ScriptModuleBase>::Find(name);
                if (!found || !found->IsScript())
                    return;

                ScriptController::Add(GetCurrentOrphanScripts()->Add(static_cast<const ::Atmos::Script*>(found.Get())->MakeInstance()));
            }

            void CreateAndStartGlobal(const Name &name, const std::vector<Variant> &globalNames, const std::vector<Variant> &globalValues)
            {
                if (globalNames.size() != globalValues.size())
                    return;

                auto found = AssetRegistry<ScriptModuleBase>::Find(name);
                if (!found || !found->IsScript())
                    return;

                ::Atmos::Script::Instance instance(static_cast<const ::Atmos::Script*>(found.Get())->MakeInstance());

                // Add global items
                {
                    auto valueLoop = globalValues.begin();
                    for (auto nameLoop = globalNames.begin(); nameLoop != globalNames.end(); ++nameLoop, ++valueLoop)
                    {
                        if (!nameLoop->IsString())
                            continue;

                        instance.AddGlobalItem(NameValuePair(nameLoop->AsString(), *valueLoop));
                    }
                }

                ScriptController::Add(GetCurrentOrphanScripts()->Add(std::move(instance)));
            }

            void CreateAndStartExternal(const Name &name, const ::Atmos::Script::SymbolName &overrideExecuteName, const std::vector<Variant> &overrideParameterNames, const std::vector<Variant> &overrideParameterValues)
            {
                auto found = AssetRegistry<ScriptModuleBase>::Find(name);
                if (!found || !found->IsScript())
                    return;

                // Setup override parameters
                ::Atmos::Script::Instance::ItemVector overrideParameters;
                {
                    auto valueLoop = overrideParameterValues.begin();
                    for (auto nameLoop = overrideParameterNames.begin(); nameLoop != overrideParameterNames.end(); ++nameLoop, ++valueLoop)
                    {
                        if (!nameLoop->IsString())
                            continue;

                        overrideParameters.push_back(NameValuePair(nameLoop->AsString(), *valueLoop));
                    }
                }

                ScriptController::Add(GetCurrentOrphanScripts()->Add(static_cast<const ::Atmos::Script*>(found.Get())->MakeInstance()), overrideExecuteName, overrideParameters);
            }

            void CreateAndStartGlobalExternal(const Name &name, const std::vector<Variant> &globalNames, const std::vector<Variant> &globalValues, const ::Atmos::Script::SymbolName &overrideExecuteName, const std::vector<Variant> &overrideParameterNames, const std::vector<Variant> &overrideParameterValues)
            {
                if (globalNames.size() != globalValues.size())
                    return;
                if (overrideParameterNames.size() != overrideParameterValues.size())
                    return;

                auto found = AssetRegistry<ScriptModuleBase>::Find(name);
                if (!found || !found->IsScript())
                    return;

                ::Atmos::Script::Instance instance(static_cast<const ::Atmos::Script*>(found.Get())->MakeInstance());

                // Add global items
                {
                    auto valueLoop = globalValues.begin();
                    for (auto nameLoop = globalNames.begin(); nameLoop != globalNames.end(); ++nameLoop, ++valueLoop)
                    {
                        if (!nameLoop->IsString())
                            continue;

                        instance.AddGlobalItem(NameValuePair(nameLoop->AsString(), *valueLoop));
                    }
                }

                // Setup override parameters
                ::Atmos::Script::Instance::ItemVector overrideParameters;
                {
                    auto valueLoop = overrideParameterValues.begin();
                    for (auto nameLoop = overrideParameterNames.begin(); nameLoop != overrideParameterNames.end(); ++nameLoop, ++valueLoop)
                    {
                        if (!nameLoop->IsString())
                            continue;

                        overrideParameters.push_back(NameValuePair(nameLoop->AsString(), *valueLoop));
                    }
                }

                ScriptController::Add(GetCurrentOrphanScripts()->Add(std::move(instance)), overrideExecuteName, overrideParameters);
            }
        }

        namespace Ent
        {
            void Move(Entity entity, const GridPosition &moveTo, TimeValue timeTaken)
            {
                ::Atmos::Ent::PositionSystem::MoveEntity(entity, moveTo, timeTaken);
            }

            void Move(Entity entity, const Direction &direction)
            {
                ::Atmos::Ent::PositionSystem::MoveEntity(entity, direction);
            }

            void SetSpriteVisibility(Entity entity, OffsetManagedList<Sprite>::ID spriteID, bool set)
            {
                auto render = GetCurrentEntities()->FindComponent<::Atmos::Ent::SenseComponent>(entity);
                if (!render)
                    return;

                auto sprite = render->FindSprite(spriteID);
                if (!sprite)
                    return;

                (*sprite)->Enable(set);
            }

            void EnableSprite(Entity entity, OffsetManagedList<Sprite>::ID spriteID)
            {
                auto render = GetCurrentEntities()->FindComponent<::Atmos::Ent::SenseComponent>(entity);
                if (!render)
                    return;

                auto sprite = render->FindSprite(spriteID);
                if (!sprite)
                    return;

                (*sprite)->Enable(true);
            }

            void DisableSprite(Entity entity, OffsetManagedList<Sprite>::ID spriteID)
            {
                auto render = GetCurrentEntities()->FindComponent<::Atmos::Ent::SenseComponent>(entity);
                if (!render)
                    return;

                auto sprite = render->FindSprite(spriteID);
                if (!sprite)
                    return;

                (*sprite)->Disable();
            }

            ::Atmos::Ent::InventoryComponent::Container::size_type GetItemCount(Entity entity)
            {
                auto inventory = GetCurrentEntities()->FindComponent<::Atmos::Ent::InventoryComponent>(entity);
                if (!inventory)
                    return 0;

                return inventory->container.size();
            }

            bool IsEntityInParty(Entity entity)
            {
                return ::Atmos::Ent::PlayerParty::IsHere(entity);
            }

            Gold GetAvatarGold()
            {
                return ::Atmos::Ent::AvatarSystem::GetAvatar()->gold;
            }

            void AddToPlayerParty(Entity entity)
            {
                ::Atmos::Ent::PlayerParty::Add(entity);
            }

            void AttachModulatorToSense(Entity entity, ::Atmos::Modulator::Controller::ID modID)
            {
                auto &obs = GameEnvironment::FindModulator(modID);
                if (!obs)
                    return;

                auto senseComponent = GetCurrentEntities()->FindComponent<::Atmos::Ent::SenseComponent>(entity);
                if (!senseComponent)
                    return;

                
            }
        }

        namespace Position
        {
            ::Atmos::Pathfinder::TileStack FindPath(const GridPosition &from, const GridPosition &to)
            {
                ::Atmos::Pathfinder::TileStack ret;
                ::Atmos::Pathfinder::FindPath(ret, from, to);
                return ret;
            }
        }

        namespace Battle
        {
            void GivePlacement(const std::vector<::Atmos::Variant> &give)
            {
                for (auto &loop : give)
                {
                    if(loop.IsGridPosition())
                        ::Atmos::battleState.GivePlacement(loop.AsGridPosition());
                }
            }

            Entity FindClosestPlayer(const GridPosition &focus)
            {
                if (!battleState.IsTop())
                    return ::Atmos::Ent::nullEntity;

                return battleState.FindClosestPlayer(focus);
            }

            Entity FindClosestMonster(const GridPosition &focus)
            {
                if (!battleState.IsTop())
                    return ::Atmos::Ent::nullEntity;

                return battleState.FindClosestMonster(focus);
            }

            void EndTurn(Entity entity)
            {
                auto battle = GetCurrentEntities()->FindComponent<::Atmos::Ent::BattleComponent>(entity);
                if (!battle)
                    return;

                battle->EndTurn();
            }
        }

        namespace Camera
        {
            void Move(const Direction &dir, float distance)
            {
                ::Atmos::Camera::Move(dir, distance);
            }

            void MoveTo(const Position2D &pos, const FrameTimer &timer)
            {
                ::Atmos::Camera::MoveTo<::affecter::SmoothstepType>(pos, timer);
            }

            void MoveToInstant(const Position2D &pos)
            {
                ::Atmos::Camera::MoveToInstant(pos);
            }

            void MoveDeltaInstant(const Position2D &delta)
            {
                ::Atmos::Camera::MoveDeltaInstant(delta);
            }

            void MoveToFromScreenPos(const Position2D &pos, const FrameTimer &timer)
            {
                ::Atmos::Camera::MoveToFromScreenPos(pos, timer);
            }
        }

        namespace Field
        {
            void Change(const Direction &dir, const GridPosition &pos, FieldID id)
            {
                WorldManager::Request(FieldDestination(dir, pos, id));
            }
        }

        namespace Music
        {
            void Change(const FileName &name)
            {
                ::Atmos::CurrentMusic::ChangePlaying(name);
            }
        }

        namespace Scheduler
        {
            void Schedule(::Atmos::Act::ID id, const std::vector<::Atmos::Variant> &args, TimeValue::ValueT timeTaken)
            {
                std::vector<Parameter> parameters;
                for (auto &loop : args)
                    parameters.push_back(std::move(loop));

                Action action(id);
                action.SetParameters(std::move(parameters));
                GetCurrentScheduler()->Schedule(std::move(action), timeTaken);
            }
        }
    }
}