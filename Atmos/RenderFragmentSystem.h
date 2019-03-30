#pragma once

#include "ObjectSystem.h"
#include "ObjectBatch.h"
#include "ObjectManager.h"

#include "RenderFragment.h"
#include "RenderFragmentGrid.h"

namespace Atmos
{
    class RenderFragmentSystem : public ObjectSystem
    {
    public:
        typedef TypedObjectReference<nRenderFragment> Reference;
    public:
        RenderFragmentSystem(ObjectManager& manager);

        void DrawAll();
    private:
        typedef ObjectBatch<nRenderFragment> RenderFragmentBatch;

        RenderFragmentBatch renderFragments;
    private:
        Octree grid;
    private:
        void OnCreated(Reference object);
    private:
        void OnObjectBoundsChanged(TypedObjectReference<PositionalObject> object, AxisBoundingBox3D previous);
    };

    template<>
    struct ObjectSystemTraits<RenderFragmentSystem>
    {
        static const ObjectSystemPriority priority = 0;
    };
}