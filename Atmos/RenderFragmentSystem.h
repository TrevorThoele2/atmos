#pragma once

#include "ObjectSystem.h"
#include "ObjectBatch.h"
#include "ObjectManager.h"

#include "RenderFragment.h"
#include "Octree.h"

namespace Atmos::Render
{
    class FragmentSystem : public ObjectSystem
    {
    public:
        typedef TypedObjectReference<Fragment> Reference;
    public:
        FragmentSystem(ObjectManager& manager);

        void DrawAll();
    private:
        typedef ObjectBatch<Fragment> RenderFragmentBatch;

        RenderFragmentBatch renderFragments;
    private:
        Octree grid;
    private:
        void OnCreated(Reference object);
    private:
        void OnObjectBoundsChanged(TypedObjectReference<AxisAlignedObject> object, AxisAlignedBox3D previous);
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::FragmentSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::FragmentSystem, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };
}