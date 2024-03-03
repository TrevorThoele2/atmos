#pragma once

#include <vector>
#include <unordered_map>

#include "EntitySystem.h"
#include "Entity.h"
#include "SenseComponent.h"
#include "GeneralComponent.h"

#include "FrameTimer.h"
#include "GridPosition.h"

#include "Event.h"
#include "Optional.h"

namespace Atmos
{
    class Field;
    namespace Ent
    {
        class MovementEventArgs
        {
        private:
            Entity entity;

            GridPosition source;
            GridPosition destination;
        public:
            MovementEventArgs(Entity entity, const GridPosition &source, const GridPosition &destination);
            Entity GetEntity() const;
            const GridPosition& GetSourcePosition() const;
            const GridPosition& GetDestinationPosition() const;
        };

        class MovementCollisionEventArgs
        {
        private:
            Entity source;
            GridPosition hitPosition;
            bool hitTile;
        public:
            MovementCollisionEventArgs(Entity source, const GridPosition &hitPosition, bool hitTile);
            Entity GetSource() const;
            const GridPosition& GetHitPosition() const;
            bool DidHitTile() const;
        };

        class GeneralComponent;
        class SenseComponent;
        class MovementComponent;
        class PositionSystem : public System<PositionSystem>
        {
        public:
            enum class CollisionType
            {
                NONE,
                TILE,
                ENTITY
            };
        private:
            typedef Modulator::Modulator<SenseComponent> SenseModulator;
        private:
            struct ModulatorEntry
            {
                Modulator::Observer moverObserver;
                SenseComponent::ModulatorPack* sensePack;
                ModulatorEntry(const Modulator::Observer &moverObserver, SenseComponent::ModulatorPack *sensePack);
                ModulatorEntry(const ModulatorEntry &arg) = default;
                ModulatorEntry& operator=(const ModulatorEntry &arg) = default;
                bool operator==(const ModulatorEntry &arg) const;
                bool operator!=(const ModulatorEntry &arg) const;

                void Stop();
            };
        private:
            typedef System<PositionSystem> SystemBaseT;
            friend SystemBaseT;

            static std::unordered_multimap<GridPosition, Entity> map;
            static std::unordered_map<Entity, ModulatorEntry> moving;

            PositionSystem() = default;
            PositionSystem(const PositionSystem &arg) = delete;
            PositionSystem& operator=(const PositionSystem &arg) = delete;

            void Init() override final;

            void OnDestroyEntity(Entity entity);
            void OnGeneralComponentCreated(GeneralComponent &component);
            void OnSenseComponentCreated(SenseComponent &component);
            void OnFinalizeField();

            void SetSenseComponentPosition(Entity entity, SenseComponent &component, const Optional<Atmos::GridPosition> &position);
            Modulator::Observer GenerateSenseComponentMover(const GeneralComponent::PositionT &curPosition, const GeneralComponent::PositionT &moveToPosition, const TimeValue &timeTaken);

            static void OnMoveEntity(Entity entity, const GridPosition &moveTo);
        public:
            static Event<MovementEventArgs> startedMoving;
            static Event<MovementEventArgs> finishedMoving;
            static Event<MovementCollisionEventArgs> collision;

            static std::vector<Entity> FindEntities(const GridPosition &position);

            static bool MoveEntity(Entity entity, const GridPosition &moveTo, TimeValue timeTaken);
            static bool MoveEntity(Entity entity, const Direction &direction);
            static bool MoveEntityInstant(Entity entity, const GridPosition &moveTo);
            static bool MoveEntityInstant(Entity entity, const RelativeGridPosition &offset);
            static bool SetDirection(Entity entity, const Direction &set);

            static bool CanMove(Entity entity);
            static bool CanMove(const MovementComponent &movement);
            static bool CanMove(Entity entity, const GridPosition &destination);
            static bool CanMove(const MovementComponent &movement, const GridPosition &destination);
            static CollisionType CheckCollision(Entity entity, const GridPosition &destination);
            static CollisionType CheckCollision(const MovementComponent &movement, const GridPosition &destination);

            static void Work();
        };
    }
}