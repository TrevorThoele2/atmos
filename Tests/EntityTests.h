#pragma once

#include "GeneralFixture.h"

#include <Atmos/Entity.h>
#include <Atmos/EntityPrototype.h>
#include <Atmos/ActualizeAllEntityPrototypes.h>
#include <Atmos/MappedEntities.h>
#include <Atmos/MoveEntityTo.h>
#include <Atmos/RealWorldManager.h>

using namespace Atmos;
using namespace Entity;

class EntityTestsFixture : public GeneralFixture
{
public:
    World::RealManager worldManager;

    std::unique_ptr<Arca::Reliquary> reliquary;
    Arca::Index<Mapped> mappedEntities;
public:
    EntityTestsFixture();
};