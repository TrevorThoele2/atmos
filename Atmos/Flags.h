#pragma once

#include <Chroma/Flags.h>
#include "Serialization.h"

namespace Atmos
{
    using ::Chroma::Flags;
}

namespace Inscription
{
    template<class Enum>
    class Scribe<::Chroma::Flags<Enum>, BinaryArchive> :
        public CompositeScribe<::Chroma::Flags<Enum>, BinaryArchive>
    {
    private:
        using BaseT = CompositeScribe<::Chroma::Flags<Enum>, BinaryArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive)
        {
            if (archive.IsOutput())
            {
                archive(object.GetBits());
            }
            else
            {
                using Bits = typename ObjectT::Bits;

                Bits bits;
                archive(bits);

                object = ObjectT(bits);
            }
        }
    };
}