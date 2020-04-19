#pragma once

#include <Arca/Shard.h>

#include <vector>

#include "DynamicImage.h"
#include "Sound.h"

#include "Position3D.h"

namespace Atmos::Entity
{
    class SenseComponent
    {
    public:
        using ImageList = std::vector<Render::DynamicImage>;
        using SoundList = std::vector<Audio::Sound>;

        ImageList images;
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
        static inline const TypeName typeName = "SenseComponent";
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Entity::SenseComponent, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Entity::SenseComponent, BinaryArchive>
    {};
}