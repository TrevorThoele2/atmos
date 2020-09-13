#pragma once

#include <Arca/Curator.h>

namespace Atmos::Frame
{
    class IdleCurator : public Arca::Curator
    {
    public:
        explicit IdleCurator(Init init);

        void Work();
    private:
        [[nodiscard]] bool IsDone();
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Frame::IdleCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline TypeName typeName = "Atmos::Frame::IdleCurator";
    };
}