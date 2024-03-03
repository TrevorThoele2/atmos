
#include "QuestPart.h"

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(QuestPart)
    {
        scribe(name);
        scribe(description);
        scribe(type);
    }
}