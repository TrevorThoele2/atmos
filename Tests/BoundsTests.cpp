#include "catch.hpp"
#include "BoundsTests.h"

#include <Arca/ReliquaryOrigin.h>
#include <Atmos/TypeRegistration.h>

SCENARIO_METHOD(BoundsTestsFixture, "bounds")
{
    GIVEN("registered reliquary")
    {
        Arca::ReliquaryOrigin reliquaryOrigin;

        Spatial::RegisterTypes(reliquaryOrigin);

        auto reliquary = reliquaryOrigin.Actualize();
    }
}