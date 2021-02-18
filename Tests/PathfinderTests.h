#pragma once

#include "GeneralFixture.h"

#include <Atmos/RealWorldManager.h>
#include <Arca/Reliquary.h>

using namespace Atmos;

class PathfinderTestsFixture : public GeneralFixture
{
public:
    World::RealManager worldManager;

    std::unique_ptr<Arca::Reliquary> reliquary;
public:
    PathfinderTestsFixture();
};