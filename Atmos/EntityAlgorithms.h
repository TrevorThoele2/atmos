#pragma once

#include <Arca/Reliquary.h>
#include "GridPoint.h"

namespace Atmos::Entity
{
    [[nodiscard]] bool DoCanMoveTo(bool isSolid, Spatial::Grid::Point to, Arca::Reliquary& reliquary);
}