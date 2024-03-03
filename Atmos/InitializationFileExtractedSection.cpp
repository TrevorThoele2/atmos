
#include "InitializationFileExtractedSection.h"

namespace Atmos::Initialization
{
    bool FileExtractedSection::IsOccupied() const
    {
        return !header.empty() && !nameValues.empty();
    }
}