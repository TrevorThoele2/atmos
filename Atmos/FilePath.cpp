
#include "FilePath.h"

namespace Inscription
{
    template<>
    void Serialize(BinaryScribe& scribe, ::Chroma::FilePath& obj)
    {
        if (scribe.IsOutput())
        {
            scribe.Save(obj.GetValue());
        }
        else // INPUT
        {
            std::string str;
            scribe.Load(str);

            obj = ::Chroma::FilePath(str);
        }
    }
}