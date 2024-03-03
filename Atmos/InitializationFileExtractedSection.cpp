
#include "InitializationFileExtractedSection.h"

namespace Atmos
{
    bool InitializationFileExtractedSection::IsOccupied() const
    {
        return !header.empty() && !nameValues.empty();
    }
}