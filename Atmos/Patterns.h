
#pragma once

#include <memory>

#include "RangePattern.h"
#include "Sprite.h"

#include "Serialization.h"

namespace Atmos
{
    class GraphicalPatternEntry
    {
    private:
        INSCRIPTION_TABLE_CONSTRUCTOR_SIMPLE_DECLARE(GraphicalPatternEntry, private);
    public:
        Sprite sprite;

        GraphicalPatternEntry(Sprite &&sprite);
        bool operator==(const GraphicalPatternEntry &arg) const;
    };

    typedef RelativeRangePattern<GraphicalPatternEntry> RelativeGraphicalPattern;
}

namespace inscription
{
    template<>
    class Inscripter<::Atmos::GraphicalPatternEntry> : public InscripterBase<::Atmos::GraphicalPatternEntry>
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
    };
}