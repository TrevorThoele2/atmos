
#include "RelativeFilePath.h"

namespace Inscription
{
    template<>
    void Serialize(BinaryScribe& scribe, ::Chroma::RelativeFilePath& obj)
    {
        if (scribe.IsOutput())
        {
            scribe.Save(obj.GetMoveUpCount());
            scribe.Save(obj.GetMoveDown());
        }
        else // INPUT
        {
            size_t moveUpCount;
            scribe.Load(moveUpCount);

            std::string moveDown;
            scribe.Load(moveDown);

            obj = ::Chroma::RelativeFilePath(moveUpCount, moveDown);
        }
    }
}