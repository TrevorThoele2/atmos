#include "GenerateRandomNumber.h"

namespace Atmos::Random
{
    bool GenerateBool::Do(Arca::Reliquary& reliquary) const
    {
        const auto information = Arca::Index<Information>(reliquary);
        return information->random.GenerateBool();
    }
}