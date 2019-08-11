#pragma once

#include "FieldID.h"

#include "StoredProperty.h"

#include "Serialization.h"

namespace Atmos::World
{
    class WorldStart
    {
    public:
        StoredProperty<FieldID> fieldID;
    public:
        WorldStart(FieldID fieldID = 0);
        WorldStart(const WorldStart& arg);
        WorldStart& operator=(const WorldStart& arg);
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::World::WorldStart, BinaryArchive> :
        public CompositeScribe<::Atmos::World::WorldStart, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
        void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override;
    };
}