#pragma once

#include <vector>

#include "nEntityComponent.h"

#include "ObjectOffset.h"
#include "Sprite.h"
#include "Sound.h"

#include "Position3D.h"

#include "StoredProperty.h"

namespace Atmos
{
    namespace Ent
    {
        class nSenseComponent : public nEntityComponent
        {
        public:
            typedef PositionalOffsetAdapter<nSprite> SpriteOffset;
            typedef PositionalOffsetAdapter<nSound> SoundOffset;

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
            nSenseComponent(EntityReference reference);
            nSenseComponent(const nSenseComponent& arg) = default;
            nSenseComponent(const ::Inscription::Table<nSenseComponent>& table);

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
    struct ObjectTraits<Ent::nSenseComponent> : ObjectTraitsBase<Ent::nSenseComponent>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::Ent::nSenseComponent)
    {
    public:
        static void AddMembers(TableT& table);

        INSCRIPTION_INSCRIPTER_DECLARE_SERIALIZE_FUNCTION;
    };
}