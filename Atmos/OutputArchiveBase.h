#pragma once

#include "ArchiveBase.h"

#include <Inscription/ContainerSize.h>

namespace Atmos
{
    class Field;

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