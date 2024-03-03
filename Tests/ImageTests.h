#pragma once

#include "GeneralFixture.h"

#include <Atmos/Logger.h>
#include <Atmos/Field.h>
#include <Atmos/MaterialAsset.h>
#include <Atmos/AxisAlignedBox2D.h>
#include <Atmos/AxisAlignedBox3D.h>
#include <Atmos/Size2D.h>
#include <Atmos/Size3D.h>

class ImageTestsFixture : public GeneralFixture
{
public:
    Atmos::Logging::Logger logger;
    Atmos::World::Field field;
    Arca::Reliquary* fieldReliquary;
    Arca::Index<Atmos::Asset::Material> materialAsset;
public:
    const Atmos::Spatial::AxisAlignedBox2D max2D =
    {
        Atmos::Spatial::Point2D{},
        Atmos::Spatial::Size2D
        {
            std::numeric_limits<Atmos::Spatial::AxisAlignedBox2D::Coordinate>::max() / 2 - 1,
            std::numeric_limits<Atmos::Spatial::AxisAlignedBox2D::Coordinate>::max() / 2 - 1
        }
    };

    const Atmos::Spatial::AxisAlignedBox3D max3D =
    {
        Atmos::Spatial::Point3D{},
        Atmos::Spatial::Size3D
        {
            std::numeric_limits<Atmos::Spatial::AxisAlignedBox2D::Coordinate>::max() / 2 - 1,
            std::numeric_limits<Atmos::Spatial::AxisAlignedBox2D::Coordinate>::max() / 2 - 1,
            std::numeric_limits<Atmos::Spatial::AxisAlignedBox2D::Coordinate>::max() / 2 - 1
        }
    };
public:
    ImageTestsFixture();
};