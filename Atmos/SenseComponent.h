#pragma once

#include <Arca/Shard.h>

#include <vector>

#include "DynamicMaterialView.h"
#include "Sound.h"

#include "Position3D.h"

namespace Atmos::Entity
{
    class SenseComponent
    {
    public:
        using MaterialViewList = std::vector<Render::DynamicMaterialView>;
        using SoundList = std::vector<Audio::Sound>;

        MaterialViewList materialViews;
        SoundList sounds;
    public:
        using Position = Position3D;
        Position position;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Entity::SenseComponent>
    {
        static const ObjectType objectType = ObjectType::Shard;
        static const TypeName typeName;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Entity::SenseComponent, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Entity::SenseComponent, BinaryArchive>
    {};
}