#pragma once

#include "Serialization.h"

namespace Atmos
{
    struct ScreenSize
    {
        using Dimension = int;
        Dimension width = 0;
        Dimension height = 0;

        ScreenSize() = default;
        ScreenSize(Dimension width, Dimension height);
        ScreenSize(const ScreenSize& arg) = default;

        bool operator==(const ScreenSize& arg) const;
        bool operator!=(const ScreenSize& arg) const;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::ScreenSize, BinaryArchive> final :
        public CompositeScribe<Atmos::ScreenSize, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}