#pragma once

#include "Serialization.h"

namespace Atmos::Asset
{
    struct ImageSize
    {
        using Dimension = int;
        Dimension width = 0;
        Dimension height = 0;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Asset::ImageSize, BinaryArchive> final :
        public CompositeScribe<Atmos::Asset::ImageSize, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override
        {
            archive(object.width);
            archive(object.height);
        }
    };
}