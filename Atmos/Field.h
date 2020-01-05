#pragma once

#include "FieldID.h"
#include <Arca/Reliquary.h>

namespace Atmos::World
{
    class Field
    {
    public:
        Field() = default;
        Field(FieldID id, std::unique_ptr<Arca::Reliquary>&& reliquary);
        Field(Field&& arg) noexcept;
        Field& operator=(Field&& arg) noexcept;

        [[nodiscard]] FieldID ID() const;
        [[nodiscard]] Arca::Reliquary& Reliquary();
        [[nodiscard]] const Arca::Reliquary& Reliquary() const;
    private:
        FieldID id = 0;
        std::unique_ptr<Arca::Reliquary> reliquary;
    private:
        INSCRIPTION_ACCESS;
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