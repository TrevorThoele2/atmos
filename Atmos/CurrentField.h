#pragma once

#include "Field.h"

#include "TileHandler.h"
#include "EntityManager.h"

#include "GroundItemHandler.h"

#include "RenderFragmentHandler.h"
#include "SoundHandler.h"

#include "Scheduler.h"
#include "OrphanScripts.h"

namespace Atmos
{
    Field* GetCurrentField();
    TileHandler* GetCurrentTiles();
    EntityManager* GetCurrentEntities();
    GroundItemHandler* GetCurrentGroundItems();
    RenderFragmentHandler* GetCurrentRenderFragments();
    SoundHandler* GetCurrentSounds();
    Scheduler* GetCurrentScheduler();
    OrphanScripts* GetCurrentOrphanScripts();
}