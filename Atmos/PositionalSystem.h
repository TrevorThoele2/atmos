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

    template<>
    struct ObjectSystemTraits<PositionalSystem>
    {
        static const ObjectSystemPriority priority = 0;
    };
}