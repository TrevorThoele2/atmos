
#include "CurrentField.h"
#include "WorldManager.h"
#include "GameEnvironment.h"

#define INDIVIDUAL_IMPL(handler)    \
auto field = GetCurrentField();     \
if (field)                          \
    return &field->handler;         \
else                                \
    return nullptr;

namespace Atmos
{
    Field* GetCurrentField()
    {
        return GameEnvironment::GetWorldManager().GetCurrentField();
    }

    ObjectManager* GetLocalObjectManager()
    {
        INDIVIDUAL_IMPL(objectManager);
    }
}

#undef INDIVIDUAL_IMPL