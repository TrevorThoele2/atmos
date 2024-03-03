
#include "CurrentField.h"
#include "WorldManager.h"

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
        return WorldManager::GetCurrentField();
    }

    TileHandler* GetCurrentTiles()
    {
        INDIVIDUAL_IMPL(tiles);
    }

    EntityManager* GetCurrentEntities()
    {
        INDIVIDUAL_IMPL(entities);
    }

    GroundItemHandler* GetCurrentGroundItems()
    {
        INDIVIDUAL_IMPL(groundItems);
    }

    RenderFragmentHandler* GetCurrentRenderFragments()
    {
        INDIVIDUAL_IMPL(renderFragments);
    }

    SoundHandler* GetCurrentSounds()
    {
        INDIVIDUAL_IMPL(sounds);
    }

    Scheduler* GetCurrentScheduler()
    {
        INDIVIDUAL_IMPL(scheduler);
    }

    OrphanScripts* GetCurrentOrphanScripts()
    {
        INDIVIDUAL_IMPL(orphanScripts);
    }
}

#undef INDIVIDUAL_IMPL