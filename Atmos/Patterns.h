
#pragma once

#include <memory>

#include "RangePattern.h"
#include "Sprite.h"

#include "Serialization.h"

namespace Atmos
{
    class GraphicalPatternEntry
    {
    public:
        typedef TypedObjectReference<nSprite> SpriteReference;
        SpriteReference sprite;
    public:
        GraphicalPatternEntry(SpriteReference sprite);
        bool operator==(const GraphicalPatternEntry &arg) const;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
    };

    typedef RelativeRangePattern<GraphicalPatternEntry> RelativeGraphicalPattern;
}