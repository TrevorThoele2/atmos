#pragma once

#include <Chroma/RelativeFilePath.h>
#include "Serialization.h"

namespace Atmos
{
    using ::Chroma::RelativeFilePath;
}

namespace Inscription
{
    template<>
    void Serialize(BinaryScribe& scribe, ::Chroma::RelativeFilePath& obj);
}