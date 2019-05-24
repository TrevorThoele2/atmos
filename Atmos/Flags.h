#pragma once

#include <Chroma/Flags.h>
#include "Serialization.h"

namespace Atmos
{
    using ::Chroma::Flags;
}

namespace Inscription
{
    template<class EnumT>
    void Serialize(Scribe& scribe, ::Chroma::Flags<EnumT>& obj)
    {
        if (scribe.IsOutput())
        {
            scribe.Save(obj.GetBits());
        }
        else
        {
            typedef typename ::Chroma::Flags<EnumT>::Bits Bits;

            Bits bits;
            scribe.Load(bits);

            obj = ::Chroma::Flags<EnumT>(bits);
        }
    }
}