#pragma once

#include "RegistryObject.h"
#include "Event.h"

namespace Atmos
{
    class AbilityBase : public RegistryObject
    {
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    public:
        AbilityBase() = default;
        AbilityBase(const AbilityBase &arg) = default;
        AbilityBase(AbilityBase &&arg);
        AbilityBase& operator=(const AbilityBase &arg) = default;
        AbilityBase& operator=(AbilityBase &&arg);

        bool operator==(const AbilityBase &arg) const;
        bool operator!=(const AbilityBase &arg) const;

        void Start() const;
        bool CanStart() const;
    };

    class Tile;
    class AbilityUsing
    {
    private:
        friend AbilityBase;
    private:
        static const AbilityBase *current;

        AbilityUsing() = default;
        AbilityUsing(const AbilityUsing &arg) = delete;
        AbilityUsing& operator=(const AbilityUsing &arg) = delete;
    public:
        Event<const AbilityBase&> eventStarted;

        static AbilityUsing& Instance();
        static void Finish(Tile &tile);
        static const AbilityBase* Current();
        static bool IsActive();
    };
}