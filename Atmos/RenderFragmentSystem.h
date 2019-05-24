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
        typedef TypedObjectReference<RenderFragment> Reference;
    public:
        RenderFragmentSystem(ObjectManager& manager);
        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(RenderFragmentSystem);

        void DrawAll();
    private:
        typedef ObjectBatch<RenderFragment> RenderFragmentBatch;

        RenderFragmentBatch renderFragments;
    private:
        Octree grid;
    private:
        void OnCreated(Reference object);
    private:
        void OnObjectBoundsChanged(TypedObjectReference<PositionalObject> object, AxisBoundingBox3D previous);
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::RenderFragmentSystem)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_BINARY_DECLARE_CLASS_NAME_RESOLVER;
    };
}