#pragma once

#include <vector>
#include "FixedPoint.h"

namespace Atmos
{
    // Calculates the value from a stat and a power rating
    FixedPoint64 CalculateValueCap(FixedPoint64 power, std::vector<FixedPoint64> stats);
}