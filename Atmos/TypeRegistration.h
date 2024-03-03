#pragma once

#include <Arca/ReliquaryOrigin.h>

namespace Atmos
{
    void RegisterGlobalTypes(Arca::ReliquaryOrigin& origin);
    void RegisterFieldTypes(Arca::ReliquaryOrigin& origin, Arca::Reliquary& globalReliquary);
}