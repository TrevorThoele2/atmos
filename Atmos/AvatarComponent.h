#pragma once

#include "EntityComponent.h"

namespace Atmos::Entity
{
    class AvatarComponent : public Component
    {
    public:
        typedef int Currency;
    public:
        Currency currency;
    public:
        AvatarComponent(ObjectManager& manager, EntityReference owner);
        AvatarComponent(const AvatarComponent& arg) = default;
        AvatarComponent(const ::Inscription::BinaryTableData<AvatarComponent>& data);

        ObjectTypeDescription TypeDescription() const override;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Atmos
{
    template<>
    struct ObjectTraits<Entity::AvatarComponent> : ObjectTraitsBase<Entity::AvatarComponent>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<Entity::Component> bases = {};
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::Entity::AvatarComponent, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::Entity::AvatarComponent, BinaryArchive>
    {};

    template<>
    class Scribe<::Atmos::Entity::AvatarComponent, BinaryArchive> :
        public ObjectScribe<::Atmos::Entity::AvatarComponent, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {};
    };
}