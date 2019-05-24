#include "FileName.h"

#include <Inscription/String.h>

namespace Inscription
{
    template<>
    void Serialize(BinaryScribe& scribe, ::Chroma::FileName& obj)
    {
        if (scribe.IsOutput())
        {
            scribe.Save(::Inscription::RemoveConst(obj.GetValue()));
        }
        else // INPUT
        {
            std::string str;
            scribe.Load(str);

            obj = ::Chroma::FileName(str);
        }
    }
}