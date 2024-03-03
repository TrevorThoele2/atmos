#pragma once

#include <Chroma/FilePath.h>
#include "Serialization.h"

namespace Atmos
{
    using ::Chroma::FilePath;
}

namespace Inscription
{
    template<>
    void Serialize(BinaryScribe& scribe, ::Chroma::FilePath& obj);
}