
#include "ScriptCaller.h"

namespace Atmos
{
    ScriptCaller::ScriptCaller()
    {
        variant.Set();
    }

    void ScriptCaller::Set()
    {
        variant.Set();
    }

    void ScriptCaller::Set(Entity set)
    {
        variant.Set(std::move(set));
    }

    bool ScriptCaller::IsEmpty() const
    {
        return !variant.IsInhabited();
    }

    bool ScriptCaller::IsEntity() const
    {
        return variant.Is<Entity>();
    }

    Entity ScriptCaller::GetEntity()
    {
        auto got = variant.GetAsRaw<Entity>();
        if (!got)
            return Ent::nullEntity;
        return *got;
    }
}