#pragma once

#include <memory>

#include "FieldID.h"
#include <Arca/Reliquary.h>

namespace Atmos::World
{
    struct Field
    {
        FieldID id = 0;
        Arca::Reliquary* reliquary = nullptr;

        Field() = default;
        explicit Field(FieldID id);
        Field(FieldID id, Arca::Reliquary& reliquary);
        Field(Field&& arg) noexcept = default;
        Field& operator=(Field&& arg) noexcept;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::World::Field, BinaryArchive> final
        : public CompositeScribe<Atmos::World::Field, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}