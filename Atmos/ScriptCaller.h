#pragma once

#include "Entity.h"

#include <Chroma/Variant.h>

namespace Atmos
{
    class ScriptCaller
    {
    public:
        enum class Type
        {
            EMPTY,
            ENTITY
        };
    private:
        typedef ::Chroma::Variant<Entity> VariantT;

        VariantT variant;
    public:
        ScriptCaller();
        void Set();
        void Set(Entity set);
        bool IsEmpty() const;
        bool IsEntity() const;
        Entity GetEntity();
    };
}