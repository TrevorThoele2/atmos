#pragma once

#include <Arca/Curator.h>

#include "TimeInformation.h"
#include "TimeSettings.h"

namespace Atmos::Time
{
    class IdleCurator : public Arca::Curator
    {
    public:
        explicit IdleCurator(Init init);

        void Work();
    private:
        Arca::Index<Information> timeInformation;
        Arca::Index<Settings> timeSettings;

        [[nodiscard]] bool IsDone() const;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Time::IdleCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline TypeName typeName = "IdleCurator";
    };
}