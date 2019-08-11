#pragma once

#include "ArchiveBase.h"

#include "Field.h"

#include <Inscription/ContainerSize.h>

namespace Atmos::World::Serialization
{
    class OutputArchiveBase : public ArchiveBase
    {
    public:
        virtual ~OutputArchiveBase() = 0;

        virtual void Save(Field& field) = 0;
        virtual void OverwriteFieldCount(::Inscription::ContainerSize set) = 0;
    protected:
        OutputArchiveBase() = default;
    };
}