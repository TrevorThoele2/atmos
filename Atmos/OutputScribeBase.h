#pragma once

#include "ScribeBase.h"

#include <Inscription/ContainerSize.h>

namespace Atmos
{
    class Field;

    class OutputScribeBase : public ScribeBase
    {
    public:
        virtual ~OutputScribeBase() = 0;

        virtual void Save(Field& field) = 0;
        virtual void OverwriteFieldCount(::Inscription::ContainerSize::ValueT set) = 0;
    protected:
        OutputScribeBase() = default;
    };
}