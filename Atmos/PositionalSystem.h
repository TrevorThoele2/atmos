#pragma once

#include "ObjectSystem.h"
#include "ObjectBatch.h"
#include "ObjectManager.h"

#include "AxisAlignedObject.h"

#include "Octree.h"

namespace Atmos
{
    class PositionalSystem : public ObjectSystem
    {
    public:
        Grid::Octree grid;
    public:
        PositionalSystem(ObjectManager& manager);
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
    template<>
    class Scribe<::Atmos::PositionalSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::PositionalSystem, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}