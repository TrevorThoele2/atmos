
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


        std::unordered_multimap<GridPosition, Entity> PositionSystem::map;
        std::unordered_set<Entity> PositionSystem::moving;

        void PositionSystem::Init()
        {
            SubscribeEvent(Manager::eventBeforeEntityDestroyed, &PositionSystem::OnDestroyEntity, *this);
            SubscribeEvent(GeneralComponent::Map::onCreated, &PositionSystem::OnGeneralComponentCreated, *this);
            SubscribeEvent(SenseComponent::Map::onCreated, &PositionSystem::OnSenseComponentCreated, *this);
            SubscribeEvent(WorldManager::Instance().eventFinalizeField, &PositionSystem::OnFinalizeField, *this);
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

        void PositionSystem::SetSenseComponentPosition(Entity entity, SenseComponent &component, const Optional<Atmos::GridPosition> &position)
        {
            if (position.IsValid())
                component.SetPosition(position.Get());
        }

        Modulator::Observer PositionSystem::GenerateSenseComponentMover(const GeneralComponent::PositionT &curPosition, const GeneralComponent::PositionT &moveToPosition, const TimeValue &timeTaken)
        {
            auto observer = GameEnvironment::GenerateModulator(Modulator::Description::SenseComponent.name);
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
        Event<MovementCollisionEventArgs> PositionSystem::collision;

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
            moving.emplace(entity);
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
            // Move immediately
            if (!modPicked->IsValid())
            {
                MoveEntityInstant(entity, moveTo);
                return true;
            }

            // Move via modulator
            // Create the mover modulator
            modPicked->Execute();
            Modulator::Observer mover(GameEnvironment::GenerateModulator(Modulator::Description::SenseComponent.name));
            // Setup the mover modulator
            switch(direction.Get())
            {
            case Direction::UP:
            case Direction::DOWN:
            {
                typedef decltype(Modulator::Description::Track::PositionY)::Type Type;
                auto moverTrack = mover->FindTrack(mover->AddTrack(GameEnvironment::GenerateModulatorTrack(Modulator::Description::SenseComponent.name, Modulator::Description::Track::PositionY.name)));
                auto node = moverTrack->FindNode(moverTrack->AddNode());
                //node->SetTimeTaken(foundMod->GetTimeTaken());

                Modulator::TrackNodeEndState endState(node->PrototypeEndState());
                endState.SetNormal(Modulator::Value(std::int64_t(moveTo.GetX())));
                node->SetEndState(endState);
                break;
            }

            case Direction::LEFT:
            case Direction::RIGHT:
            {
                typedef decltype(Modulator::Description::Track::PositionX)::Type Type;
                auto moverTrack = mover->FindTrack(mover->AddTrack(GameEnvironment::GenerateModulatorTrack(Modulator::Description::SenseComponent.name, Modulator::Description::Track::PositionX.name)));
                auto node = moverTrack->FindNode(moverTrack->AddNode());
                //node->SetTimeTaken(foundMod->GetTimeTaken());

                Modulator::TrackNodeEndState endState(node->PrototypeEndState());
                endState.SetNormal(Modulator::Value(std::int64_t(moveTo.GetY())));
                node->SetEndState(endState);
                break;
            }
            }

            //moving.emplace(entity, ModulatorEntry(mover, foundMod));
            static_cast<SenseModulator*>(mover.Get())->Start(*senseComponent);
            // Set direction of the general component
            generalComponent->direction = direction;
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
            return movement.CanMove();
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
                Entity entity = movement.GetOwnerEntity();
                auto &foundEntityGroup = FindEntities(destination);
                for (auto &loop : foundEntityGroup)
                {
                    auto found = GetCurrentEntities()->FindComponent<GeneralComponent>(entity);
                    if (found && found->IsSolid())
                        return CollisionType::ENTITY;
                }
            }

            // Is not colliding with anything
            return CollisionType::NONE;
        }

        void PositionSystem::Work()
        {
            for (auto loop = moving.begin(); loop != moving.end();)
            {
                auto comps = GetCurrentEntities()->FindMultipleComponents<GeneralComponent*, MovementComponent*, SenseComponent*>(*loop);
                auto general = std::get<0>(comps);
                auto movement = std::get<1>(comps);
                auto sense = std::get<2>(comps);

                if (sense->position != general->position)
                {
                    // Check for collision
                    {
                        CollisionType col = CheckCollision(*movement, sense->position);
                        if (col != CollisionType::NONE)
                        {
                            if (col == CollisionType::TILE)
                                collision(MovementCollisionEventArgs(*loop, sense->position, true));
                            else
                                collision(MovementCollisionEventArgs(*loop, sense->position, false));

                            loop = moving.erase(loop);
                            continue;
                        }
                    }

                    general->position = sense->position;
                    OnMoveEntity(*loop, general->position);
                }
                else
                {
                    /*
                    if (!loop->moverObserver->IsWorking())
                    {
                        loop = moving.erase(loop);
                        continue;
                    }
                    */
                }

                ++loop;
            }
        }

        ENTITY_SYSTEM_FORCE_INSTANTIATION(PositionSystem);
    }
}