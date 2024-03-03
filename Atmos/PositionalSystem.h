#pragma once

#include "ObjectSystem.h"
#include "ObjectBatch.h"
#include "ObjectManager.h"

#include "AxisAlignedObject.h"

#include "RenderFragmentGrid.h"

namespace Atmos
{
    class PositionalSystem : public ObjectSystem
    {
    public:
        Octree grid;
    public:
        PositionalSystem(ObjectManager& manager);
        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(PositionalSystem);
    private:
        typedef ObjectBatch<AxisAlignedObject> Batch;
        typedef TypedObjectReference<AxisAlignedObject> Reference;
    private:
        Batch batch;
    private:
        void OnCreated(Reference object);
    private:
        void OnObjectBoundsChanged(Reference object, AxisAlignedBox3D previous);
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::PositionalSystem)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_BINARY_DECLARE_CLASS_NAME_RESOLVER;
    };
}