#pragma once

#include <Arca/Curator.h>

#include "CurrentAvatar.h"
#include "SetAvatar.h"

#include "DebugValue.h"

namespace Atmos::Entity
{
    class AvatarCurator final : public Arca::Curator
    {
    public:
        explicit AvatarCurator(Init init);

        void Work();

        void Handle(const SetAvatar& command);
    private:
        Arca::Index<CurrentAvatar> currentAvatar;
    private:
        Debug::Value debugPlayerColumn;
        Debug::Value debugPlayerRow;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::AvatarCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "Atmos::Entity::AvatarCurator";
        using HandledCommands = Arca::HandledCommands<Atmos::Entity::SetAvatar>;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Entity::AvatarCurator, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Entity::AvatarCurator, BinaryArchive>
    {};
}