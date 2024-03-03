#pragma once

#include "ObjectSystem.h"
#include "ObjectBatch.h"
#include "ObjectManager.h"

#include "PositionalObject.h"

#include "RenderFragmentGrid.h"

namespace Atmos
{
    class PositionalSystem : public ObjectSystem
    {
    public:
        Octree grid;
    public:
        PositionalSystem(ObjectManager& manager);
        PositionalSystem(const ::Inscription::Table<PositionalSystem>& table);
    private:
        typedef ObjectBatch<PositionalObject> Batch;
        typedef TypedObjectReference<PositionalObject> Reference;
    private:
        Batch batch;
    private:
        void OnCreated(Reference object);
    private:
        void OnObjectBoundsChanged(Reference object, AxisBoundingBox3D previous);
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::PositionalSystem)
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
    };
}