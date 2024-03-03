
#include "EntityPositionSystem.h"

#include "GeneralComponent.h"
#include "MovementComponent.h"
#include "SenseComponent.h"
#include "CombatComponent.h"
#include "BattleComponent.h"
#include "WorldManager.h"

#include "CurrentField.h"

#include "GameEnvironment.h"

#include "Movement.h"
#include "Animation.h"
#include "ModulatorDescribers.h"

#include "Logger.h"

namespace Atmos
{
    namespace Ent
    {
        MovementEventArgs::MovementEventArgs(Entity entity, const GridPosition &source, const GridPosition &destination) : entity(entity), source(source), destination(destination)
        {}

        Entity MovementEventArgs::GetEntity() const
        {
            return entity;
        }

        const GridPosition& MovementEventArgs::GetSourcePosition() const
        {
            return source;
        }

        const GridPosition& MovementEventArgs::GetDestinationPosition() const
        {
            return destination;
        }

        MovementCollisionEventArgs::MovementCollisionEventArgs(Entity source, const GridPosition &hitPosition, bool hitTile) : source(source), hitPosition(hitPosition), hitTile(hitTile)
        {}

        Entity MovementCollisionEventArgs::GetSource() const
        {
            return source;
        }

        const GridPosition& MovementCollisionEventArgs::GetHitPosition() const
        {
            return hitPosition;
        }

        bool MovementCollisionEventArgs::DidHitTile() const
        {
            return hitTile;
        }

        PositionSystem::StagedPosition::StagedPosition(const GridPosition &position, Direction direction, Entity entity) : position(position), direction(direction), entity(entity)
        {}

        std::unordered_multimap<GridPosition, Entity> PositionSystem::map;
        Optional<PositionSystem::StagedPosition> PositionSystem::stagedPosition;
        std::unordered_map<Entity, PositionSystem::SenseModulator*> PositionSystem::movingEntities;

        void PositionSystem::Init()
        {
            SubscribeEvent(Manager::eventBeforeEntityDestroyed, &PositionSystem::OnDestroyEntity, *this);
            SubscribeEvent(GeneralComponent::Map::onCreated, &PositionSystem::OnGeneralComponentCreated, *this);
            SubscribeEvent(SenseComponent::Map::onCreated, &PositionSystem::OnSenseComponentCreated, *this);
            SubscribeEvent(WorldManager::Instance().eventFinalizeField, &PositionSystem::OnFinalizeField, *this);
            SubscribeEvent(Modulator::Controller::onStarted, &PositionSystem::OnModulatorStarted, *this);
            SubscribeEvent(Modulator::Controller::onStopped, &PositionSystem::OnModulatorStopped, *this);
        }

        void PositionSystem::OnDestroyEntity(Entity entity)
        {
            for (auto loop = map.begin(); loop != map.end(); ++loop)
            {
                if (loop->second == entity)
                {
                    map.erase(loop);
                    return;
                }
            }
        }

        void PositionSystem::OnGeneralComponentCreated(GeneralComponent &component)
        {
            map.emplace(component.position, component.GetOwnerEntity());

            auto foundSense = GetCurrentEntities()->FindComponent<SenseComponent>(component.GetOwnerEntity());
            if(foundSense)
                SetSenseComponentPosition(component.GetOwnerEntity(), *foundSense, component.position);
        }

        void PositionSystem::OnSenseComponentCreated(SenseComponent &component)
        {
            SetSenseComponentPosition(component.GetOwnerEntity(), component, GetCurrentEntities()->GetPosition(component.GetOwnerEntity()));
        }

        void PositionSystem::OnFinalizeField()
        {
            map.clear();
        }

        void PositionSystem::OnModulatorStarted(Modulator::ModulatorBase *modulator)
        {
            // If the modulator is NOT a sense component modulator
            if (modulator->GetObjectType() != typeid(SenseModulator::ObjectT))
                // Leave
                return;

            // Get sense modulator, entity, and general component
            SenseModulator *senseModulator = static_cast<SenseModulator*>(modulator);
            ATMOS_ASSERT(senseModulator->GetObject());
            Entity entity = senseModulator->GetObject()->GetOwnerEntity();
            GeneralComponent *generalComponent = GetCurrentEntities()->FindComponent<GeneralComponent>(entity);

            // Assert that the staged position is valid
            ATMOS_ASSERT(stagedPosition.IsValid());

            // Set direction of the general component
            generalComponent->direction = stagedPosition->direction;
            generalComponent->SetPosition(stagedPosition->position);
            OnMoveEntity(entity, stagedPosition->position);

            movingEntities.emplace(senseModulator->GetObject()->GetOwnerEntity(), senseModulator);
        }

        void PositionSystem::OnModulatorStopped(Modulator::ModulatorBase *modulator)
        {
            // If the modulator is NOT a sense component modulator
            if (modulator->GetObjectType() != typeid(SenseModulator::ObjectT))
                // Leave
                return;

            SenseModulator *senseModulator = static_cast<SenseModulator*>(modulator);
            if (!senseModulator->GetObject())
                return;

            movingEntities.erase(senseModulator->GetObject()->GetOwnerEntity());
        }

        void PositionSystem::SetSenseComponentPosition(Entity entity, SenseComponent &component, const Optional<Atmos::GridPosition> &position)
        {
            if (position.IsValid())
                component.SetPosition(position.Get());
        }

        Modulator::Observer PositionSystem::GenerateSenseComponentMover(const GeneralComponent::PositionT &curPosition, const GeneralComponent::PositionT &moveToPosition, const TimeValue &timeTaken)
        {
            auto observer = GameEnvironment::GenerateModulator(Modulator::Description::SenseComponent.name);
            /*
            // Setup X part of the modulator if needed
            if (curPosition.GetX() != moveToPosition.GetX())
            {
                auto track = observer->FindTrack(observer->AddTrack(GameEnvironment::GenerateModulatorTrack(Modulator::Description::SenseComponent.name, Modulator::Description::Track::PositionX.name)));
                auto node = track->FindNode(track->AddNode());
                node->SetTimeTaken(timeTaken);

                Modulator::TrackNodeEndState endState(node->PrototypeEndState());
                endState.SetNormal(Modulator::Value(std::int64_t(moveToPosition.GetX())));
                node->SetEndState(endState);
            }

            // Setup Y part of the modulator if needed
            if (curPosition.GetY() != moveToPosition.GetY())
            {
                auto track = observer->FindTrack(observer->AddTrack(GameEnvironment::GenerateModulatorTrack(Modulator::Description::SenseComponent.name, Modulator::Description::Track::PositionY.name)));
                auto node = track->FindNode(track->AddNode());
                node->SetTimeTaken(timeTaken);

                Modulator::TrackNodeEndState endState(node->PrototypeEndState());
                endState.SetNormal(Modulator::Value(std::int64_t(moveToPosition.GetY())));
                node->SetEndState(endState);
            }

            // Setup Y part of the modulator if needed
            if (curPosition.GetZ() != moveToPosition.GetZ())
            {
                auto track = observer->FindTrack(observer->AddTrack(GameEnvironment::GenerateModulatorTrack(Modulator::Description::SenseComponent.name, Modulator::Description::Track::PositionY.name)));
                auto node = track->FindNode(track->AddNode());
                node->SetTimeTaken(timeTaken);

                Modulator::TrackNodeEndState endState(node->PrototypeEndState());
                endState.SetNormal(Modulator::Value(std::int64_t(moveToPosition.GetZ())));
                node->SetEndState(endState);
            }
            */

            return observer;
        }

        void PositionSystem::OnMoveEntity(Entity entity, const GridPosition &moveTo)
        {
            for (auto loop = map.begin(); loop != map.end(); ++loop)
            {
                if (loop->second == entity)
                {
                    map.emplace(moveTo, entity);
                    map.erase(loop);
                    return;
                }
            }
        }

        Event<MovementEventArgs> PositionSystem::startedMoving;
        Event<MovementEventArgs> PositionSystem::finishedMoving;

        std::vector<Entity> PositionSystem::FindEntities(const GridPosition &position)
        {
            typedef std::vector<Entity> Ret;

            Ret ret;

            auto found = map.equal_range(position);
            if (found.first == found.second)
                return ret;

            for (auto loop = found.first; loop != found.second; ++loop)
                ret.push_back(loop->second);

            return ret;
        }

        bool PositionSystem::MoveEntity(Entity entity, const GridPosition &moveTo, TimeValue timeTaken)
        {
            if (!GetCurrentEntities()->CheckEntityExists(entity))
                return false;

            auto &components = GetCurrentEntities()->FindMultipleComponents<GeneralComponent*, SenseComponent*, MovementComponent*>(entity);
            if (!AllComponentsValid(components))
                return false;

            auto generalComponent = std::get<0>(components);
            auto senseComponent = std::get<1>(components);
            auto movementComponent = std::get<2>(components);

            if (generalComponent->position == moveTo)
                return false;

            if (!CanMove(*movementComponent, moveTo))
                return false;

            // Generate sense component modulator
            auto senseComponentMover = Instance().GenerateSenseComponentMover(generalComponent->position, moveTo, timeTaken);
            // Start the modulator
            static_cast<Modulator::Modulator<decltype(Modulator::Description::SenseComponent)::Type>*>(senseComponentMover.Get())->Start(*senseComponent);
            return true;
        }

        bool PositionSystem::MoveEntity(Entity entity, const Direction &direction)
        {
            if (!GetCurrentEntities()->CheckEntityExists(entity))
                return false;

            auto &components = GetCurrentEntities()->FindMultipleComponents<GeneralComponent*, SenseComponent*, MovementComponent*>(entity);
            if (!AllComponentsValid(components))
                return false;

            auto generalComponent = std::get<0>(components);
            auto senseComponent = std::get<1>(components);
            auto movementComponent = std::get<2>(components);

            // Figure out animate piece
            Script::Instance *modPicked = nullptr;
            switch (direction.Get())
            {
            case Direction::UP:
                modPicked = &movementComponent->upMod;
                break;
            case Direction::DOWN:
                modPicked = &movementComponent->downMod;
                break;
            case Direction::LEFT:
                modPicked = &movementComponent->leftMod;
                break;
            case Direction::RIGHT:
                modPicked = &movementComponent->rightMod;
                break;
            }

            GridPosition &position = ::Atmos::GetCurrentEntities()->GetPosition(entity).Get();
            auto &moveTo = position.FindPositionAdjacent(direction);

            if (!CanMove(*movementComponent, moveTo))
                return false;

            // Move render component
            // Move immediately if the modulator picked is invalid
            if (!modPicked->IsValid())
            {
                MoveEntityInstant(entity, moveTo);
                return true;
            }

            stagedPosition.Set(StagedPosition(moveTo, direction, entity));
            modPicked->ExecuteImmediately();
            stagedPosition.Reset();
            return true;
        }

        bool PositionSystem::MoveEntityInstant(Entity entity, const GridPosition &moveTo)
        {
            if (!WorldManager::GetCurrentField()->entities.CheckEntityExists(entity))
                return false;

            auto &components = GetCurrentEntities()->FindMultipleComponents<SenseComponent*, GeneralComponent*>(entity);
            auto senseComponent = std::get<0>(components);
            auto generalComponent = std::get<1>(components);

            if (!generalComponent)
                return false;

            generalComponent->SetPosition(moveTo);
            if(senseComponent)
                senseComponent->SetPosition(moveTo);

            OnMoveEntity(entity, moveTo);
            return true;
        }

        bool PositionSystem::MoveEntityInstant(Entity entity, const RelativeGridPosition &offset)
        {
            if (!WorldManager::GetCurrentField()->entities.CheckEntityExists(entity))
                return false;

            auto &components = GetCurrentEntities()->FindMultipleComponents<SenseComponent*, GeneralComponent*>(entity);
            auto senseComponent = std::get<0>(components);
            auto generalComponent = std::get<1>(components);

            if (!generalComponent)
                return false;

            GridPosition moveTo(generalComponent->position, offset);
            generalComponent->SetPosition(moveTo);
            if (senseComponent)
                senseComponent->SetPosition(moveTo);

            OnMoveEntity(entity, moveTo);
            return true;
        }

        bool PositionSystem::SetDirection(Entity entity, const Direction &set)
        {
            if (!GetCurrentEntities()->CheckEntityExists(entity))
                return false;

            auto &components = GetCurrentEntities()->FindMultipleComponents<SenseComponent*, GeneralComponent*>(entity);
            if (!AllComponentsValid(components))
                return false;

            auto generalComponent = std::get<1>(components);
            generalComponent->direction = set;
            return true;
        }

        bool PositionSystem::CanMove(Entity entity)
        {
            auto found = GetCurrentEntities()->FindComponent<MovementComponent>(entity);
            if (!found)
                return false;

            return CanMove(*found);
        }

        bool PositionSystem::CanMove(const MovementComponent &movement)
        {
            // If the movement can actually move and the entity is not within the moving entities
            return movement.CanMove() && (movingEntities.find(movement.GetOwnerEntity()) == movingEntities.end());
        }

        bool PositionSystem::CanMove(Entity entity, const GridPosition &destination)
        {
            auto found = GetCurrentEntities()->FindComponent<MovementComponent>(entity);
            if (!found)
                return false;

            return CanMove(*found, destination);
        }

        bool PositionSystem::CanMove(const MovementComponent &movement, const GridPosition &destination)
        {
            // If can't move generally, then don't move
            if (!CanMove(movement))
                return false;

            // If this will collide, then don't move
            if (CheckCollision(movement, destination) != CollisionType::NONE)
                return false;

            Entity entity = movement.GetOwnerEntity();
            // Check if this entity has a combat component and a battle component
            // If it does, then check if we're moving outside of the movement range
            {
                auto battle = GetCurrentEntities()->FindComponent<BattleComponent>(entity);
                if (battle)
                {
                    auto combat = GetCurrentEntities()->FindComponent<CombatComponent>(entity);
                    if (combat && battle->startingPos.FindDistance(destination) > combat->movementRange)
                        return false;
                }
            }

            return true;
        }

        PositionSystem::CollisionType PositionSystem::CheckCollision(Entity entity, const GridPosition &destination)
        {
            auto found = GetCurrentEntities()->FindComponent<MovementComponent>(entity);
            if (!found)
                return CollisionType::NONE;

            return CheckCollision(*found, destination);
        }

        PositionSystem::CollisionType PositionSystem::CheckCollision(const MovementComponent &movement, const GridPosition &destination)
        {
            Entity focusedEntity = movement.GetOwnerEntity();
            // If the focused entity is not solid, then there is no collision
            auto focusedEntityGeneral = GetCurrentEntities()->FindComponent<GeneralComponent>(focusedEntity);
            ATMOS_ASSERT_MESSAGE(focusedEntityGeneral, "The general component for the entity must exist.");
            if (!focusedEntityGeneral->IsSolid())
                return CollisionType::NONE;

            // If the tile going to does not exist, collision
            // If the tile is solid, collision
            {
                auto found = GetCurrentTiles()->Find(destination);
                if (!found || found->IsSolid())
                    return CollisionType::TILE;
            }

            // Check solidity on entities
            // If any entity we're moving into is solid, collision
            {
                
                auto &foundEntityGroup = FindEntities(destination);
                for (auto &loop : foundEntityGroup)
                {
                    auto foundLoopGeneralComponent = GetCurrentEntities()->FindComponent<GeneralComponent>(loop);
                    if (foundLoopGeneralComponent && foundLoopGeneralComponent->IsSolid())
                        return CollisionType::ENTITY;
                }
            }

            // Is not colliding with anything
            return CollisionType::NONE;
        }

        void PositionSystem::Work()
        {
            for (auto loop = movingEntities.begin(); loop != movingEntities.end(); ++loop)
            {
                auto comps = GetCurrentEntities()->FindMultipleComponents<GeneralComponent*, MovementComponent*, SenseComponent*>(loop->first);
                auto general = std::get<0>(comps);
                auto movement = std::get<1>(comps);
                auto sense = std::get<2>(comps);

                OnMoveEntity(loop->first, general->position);
            }
        }

        size_t PositionSystem::GetWorkedSize()
        {
            return movingEntities.size();
        }

        Optional<PositionSystem::StagedPosition> PositionSystem::GetStagedPosition(Entity entity)
        {
            typedef Optional<StagedPosition> RetT;
            if(!stagedPosition)
                return RetT();

            if (stagedPosition->entity != entity)
                return RetT();

            return stagedPosition;
        }

        ENTITY_SYSTEM_FORCE_INSTANTIATION(PositionSystem);
    }
}