#pragma once

#include <vector>

#include "EntityComponent.h"

#include "Sprite.h"
#include "Sound.h"
#include "PositionalOffsetAdapter.h"

#include "Position3D.h"

#include "StoredProperty.h"

namespace Atmos::Entity
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
        using Position = Position3D;
        StoredProperty<Position> position;
    public:
        SpriteList sprites;
        SoundList sounds;
    public:
        SenseComponent(ObjectManager& manager, EntityReference reference);
        SenseComponent(const SenseComponent& arg) = default;
        SenseComponent(const ::Inscription::BinaryTableData<SenseComponent>& data);

        void Enable(bool enable = true);
        void Disable();
        bool IsEnabled() const;
        // If show is true, then this will add the objects to the render fragment handler
        // If show is false, then this will remove the objects from the render fragment handler
        // It will also show/hide all of the individual objects
        void SyncObjects();

        ObjectTypeDescription TypeDescription() const override;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Atmos
{
    template<>
    struct ObjectTraits<Entity::SenseComponent> : ObjectTraitsBase<Entity::SenseComponent>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<Entity::Component> bases = {};
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::Entity::SenseComponent, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::Entity::SenseComponent, BinaryArchive>
    {
        ObjectT::Position position;
        ObjectT::SpriteList sprites;
        ObjectT::SoundList sounds;
        bool enabled;
    };

    template<>
    class Scribe<::Atmos::Entity::SenseComponent, BinaryArchive> :
        public ObjectScribe<::Atmos::Entity::SenseComponent, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {
        public:
            Table();
        protected:
            void ObjectScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
        };
    };
}