#pragma once

#include <Chroma/FileName.h>
#include "Serialization.h"

namespace Atmos
{
    using ::Chroma::FileName;
}

namespace Inscription
{
    template<>
    void Serialize(BinaryScribe& scribe, ::Chroma::FileName& obj);
}