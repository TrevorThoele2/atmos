#pragma once

#include "Item.h"
#include "ObjectReference.h"
#include "RandomAccessSequence.h"

namespace Atmos
{
    typedef RandomAccessSequence<TypedObjectReference<nItem>> ItemStash;
    typedef ItemStash::size_type ItemStashSize;
}