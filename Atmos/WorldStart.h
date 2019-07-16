
#pragma once

#include "FieldID.h"

#include "StoredProperty.h"

#include "Serialization.h"

namespace Atmos
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
    class Scribe<::Atmos::WorldStart, BinaryArchive> :
        public CompositeScribe<::Atmos::WorldStart, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };
}