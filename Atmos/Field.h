
#pragma once

#include <vector>

#include "FieldID.h"
#include "RenderFragmentHandler.h"
#include "LightingHandler.h"
#include "EntityManager.h"
#include "TileHandler.h"
#include "SoundHandler.h"
#include "GroundItemHandler.h"
#include "Scheduler.h"
#include "OrphanScripts.h"

#include "Serialization.h"

namespace Atmos
{
    class Field
    {
    public:
        typedef FieldID ID;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        ID id;

        std::vector<FieldComponent*> componentVector;
        void SetupVector();
    public:
        TileHandler tiles;
        EntityManager entities;

        GroundItemHandler groundItems;

        RenderFragmentHandler renderFragments;
        LightingHandler lighting;
        SoundHandler sounds;

        Scheduler scheduler;
        OrphanScripts orphanScripts;

        Field(ID id);
        Field(Field &&arg);
        Field& operator=(Field &&arg);

        ID GetID() const;
        void OnFinalizeField();
        void OnBeforeFieldSet(Field &newField);
        void OnAfterFieldSet();
    };
}