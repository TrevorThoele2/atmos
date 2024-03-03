
#include "FilePath.h"

namespace Inscription
{
    template<>
    void Serialize(BinaryScribe& scribe, ::Chroma::FilePath& obj)
    {
        if (scribe.IsOutput())
        {
            auto& outputScribe = *scribe.AsOutput();

            outputScribe.Save(obj.GetValue());
        }
        else // INPUT
        {
            auto& inputScribe = *scribe.AsInput();

            std::string str;
            inputScribe.Load(str);

            obj = ::Chroma::FilePath(str);
        }
    }
}