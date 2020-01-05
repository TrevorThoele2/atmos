#pragma once

#include <Arca/Curator.h>

#include "AvatarComponent.h"
#include "CurrentAvatar.h"

namespace Atmos::Debug
{
    class Statistics;
}

namespace Atmos::Entity
{
    class AvatarCurator final : public Arca::Curator
    {
    protected:
        void InitializeImplementation() override;
        void WorkImplementation(Stage& stage) override;
    private:
        CurrentAvatar* currentAvatar = nullptr;
    private:
        Debug::Statistics* debugStatistics = nullptr;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::AvatarCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "AvatarCurator";
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Entity::AvatarCurator, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Entity::AvatarCurator, BinaryArchive>
    {};
}