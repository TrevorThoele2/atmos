
#include "RelativeFilePath.h"

namespace Inscription
{
    template<>
    void Serialize(BinaryScribe& scribe, ::Chroma::RelativeFilePath& obj)
    {
        if (scribe.IsOutput())
        {
            auto& outputScribe = *scribe.AsOutput();

            outputScribe.Save(obj.GetMoveUpCount());
            outputScribe.Save(obj.GetMoveDown());
        }
        else // INPUT
        {
            auto& inputScribe = *scribe.AsInput();

            size_t moveUpCount;
            inputScribe.Load(moveUpCount);

            std::string moveDown;
            inputScribe.Load(moveDown);

            obj = ::Chroma::RelativeFilePath(moveUpCount, moveDown);
        }
    }
}