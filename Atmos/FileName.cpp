#include "FileName.h"

#include <Inscription/String.h>

namespace Inscription
{
    template<>
    void Serialize(BinaryScribe& scribe, ::Chroma::FileName& obj)
    {
        if (scribe.IsOutput())
        {
            auto& outputScribe = *scribe.AsOutput();

            outputScribe.Save(::Inscription::RemoveConst(obj.GetValue()));
        }
        else // INPUT
        {
            auto& inputScribe = *scribe.AsInput();

            std::string str;
            inputScribe.Load(str);

            obj = ::Chroma::FileName(str);
        }
    }
}