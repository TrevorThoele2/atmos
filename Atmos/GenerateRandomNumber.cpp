#include "GenerateRandomNumber.h"

namespace Atmos::Random
{
    bool GenerateBool::Do(Arca::Reliquary& reliquary) const
    {
        const auto information = reliquary.Find<Information>();
        return information->random.GenerateBool();
    }
}