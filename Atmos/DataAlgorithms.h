#pragma once

#include "Property.h"

namespace Atmos
{
    void ApplyPropertyModifications(
        std::vector<Property>& properties,
        const std::vector<Property>& add,
        const std::vector<String>& remove,
        const std::vector<Property>& replace);
}