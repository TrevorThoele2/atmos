#pragma once

#include <Arca/Curator.h>

#include "PositionedEntities.h"
#include "MoveEntity.h"
#include "MoveEntityInstant.h"
#include "MoveEntityDirection.h"
#include "RotateEntity.h"

#include "Entity.h"

#include "GridPoint.h"

namespace Atmos::Entity
{
    class PositionCurator final : public Arca::Curator
    {
    public:
        explicit PositionCurator(Init init);
    public:
        void Handle(const MoveEntity& command);
        void Handle(const MoveEntityInstant& command);
        void Handle(const MoveEntityDirection& command);
        void Handle(const RotateEntity& command);
    private:
        Arca::Index<PositionedEntities> positionedEntities;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::PositionCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "Atmos::Entity::PositionCurator";
        using HandledCommands = HandledCommands<
            Atmos::Entity::MoveEntity,
            Atmos::Entity::MoveEntityInstant,
            Atmos::Entity::MoveEntityDirection,
            Atmos::Entity::RotateEntity>;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Entity::PositionCurator, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Entity::PositionCurator, BinaryArchive>
    {};
}