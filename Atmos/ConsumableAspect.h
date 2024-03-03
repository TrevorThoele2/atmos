#pragma once

#include "ItemAspect.h"
#include "Script.h"
#include "AssetReference.h"

#include "Entity.h"

#include "Serialization.h"

namespace Atmos
{
    class ConsumableAspect : public ItemAspect
    {
    public:
        typedef unsigned char Uses;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    public:
        AssetReference<ScriptModuleBase> script;

        ConsumableAspect() = default;
        ConsumableAspect(const ConsumableAspect &arg) = default;
        ConsumableAspect& operator=(const ConsumableAspect &arg) = default;
        ConsumableAspect(ConsumableAspect &&arg);
        ConsumableAspect& operator=(ConsumableAspect &&arg);

        bool operator==(const ConsumableAspect &arg) const;
        bool operator!=(const ConsumableAspect &arg) const;

        void Affect(Entity affect) const;
    };
}