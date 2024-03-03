#pragma once

#include "Item.h"
#include "RegistryObjectReference.h"
#include "RandomAccessSequence.h"

namespace Atmos
{
    // Each "stack" is only one item, so store as item pointers
    // Used in CombatComponents
    typedef RandomAccessSequence<RegistryObjectReference<Item>> ItemStash;
    typedef ItemStash::size_type ItemStashSize;
}