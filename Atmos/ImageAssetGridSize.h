#pragma once

#include "Serialization.h"

namespace Atmos::Asset
{
    struct ImageGridSize
    {
        using Dimension = int;
        Dimension columns = 0;
        Dimension rows = 0;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Asset::ImageGridSize, BinaryArchive> final :
        public CompositeScribe<Atmos::Asset::ImageGridSize, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override
        {
            archive(object.columns);
            archive(object.rows);
        }
    };
}