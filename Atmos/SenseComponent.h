#pragma once

#include <vector>

#include "EntityComponent.h"

#include "Sprite.h"
#include "Sound.h"
#include "PositionalOffsetAdapter.h"

#include "Position3D.h"

#include "StoredProperty.h"

namespace Atmos
{
    namespace Entity
    {
        class SenseComponent : public Component
        {
        public:
            typedef PositionalOffsetAdapter<Sprite> SpriteOffset;
            typedef PositionalOffsetAdapter<Sound> SoundOffset;

            typedef std::vector<SpriteOffset> SpriteList;
            typedef std::vector<SoundOffset> SoundList;
        public:
            bool enabled;
        public:
            StoredProperty<Position3D> position;
        public:
            SpriteList sprites;
            SoundList sounds;
        public:
            SenseComponent(ObjectManager& manager, EntityReference reference);
            SenseComponent(const SenseComponent& arg) = default;
            SenseComponent(const ::Inscription::Table<SenseComponent>& table);

            void Enable(bool enable = true);
            void Disable();
            bool IsEnabled() const;
            // If show is true, then this will add the objects to the render fragment handler
            // If show is false, then this will remove the objects from the render fragment handler
            // It will also show/hide all of the individual objects
            void SyncObjects();

            ObjectTypeDescription TypeDescription() const override;
        private:
            INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        };
    }

    template<>
    struct ObjectTraits<Entity::SenseComponent> : ObjectTraitsBase<Entity::SenseComponent>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::Entity::SenseComponent)
    {
    public:
        OBJECT_INSCRIPTER_DECLARE_MEMBERS;

        INSCRIPTION_INSCRIPTER_DECLARE_SERIALIZE_FUNCTION;
    };
}