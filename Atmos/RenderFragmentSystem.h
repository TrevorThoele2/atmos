#pragma once

#include "ObjectSystem.h"
#include "ObjectBatch.h"
#include "ObjectManager.h"

#include "RenderFragment.h"
#include "Octree.h"

namespace Atmos::Render
{
    class RenderFragmentSystem : public ObjectSystem
    {
    public:
        typedef TypedObjectReference<RenderFragment> Reference;
    public:
        RenderFragmentSystem(ObjectManager& manager);

        void DrawAll();
    private:
        typedef ObjectBatch<RenderFragment> RenderFragmentBatch;

        RenderFragmentBatch renderFragments;
    private:
        Grid::Octree grid;
    private:
        void OnCreated(Reference object);
    private:
        void OnObjectBoundsChanged(TypedObjectReference<AxisAlignedObject> object, AxisAlignedBox3D previous);
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Render::RenderFragmentSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::Render::RenderFragmentSystem, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}