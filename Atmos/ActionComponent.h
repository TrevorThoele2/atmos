#pragma once

#include "EntityComponent.h"

#include "ScriptInstance.h"

#include "ObjectScribe.h"

namespace Atmos:: Entity
{
    class ActionComponent : public Component
    {
    public:
        enum class Activator : std::int32_t
        {
            EnterTile,
            AttemptEnterTile,
            UseOn,
            UseInto,
            EnterField,
            LeaveField
        };
    public:
        typedef TypedObjectReference<Script::ScriptInstance> ScriptInstanceReference;
    public:
        Activator activator;
        ScriptInstanceReference script;

        ActionComponent(ObjectManager& manager, EntityReference owner);
        ActionComponent(const ActionComponent& arg) = default;
        ActionComponent(const ::Inscription::BinaryTableData<ActionComponent>& data);

        void FireMovedInto();
        void FireAttemptMovedInto();
        void FireUseOn();
        void FireUseInto();
        void FireFieldEntered();
        void FireFieldLeft();

        ObjectTypeDescription TypeDescription() const override;
    private:
        void SetupScripts();
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Atmos
{
    template<>
    struct ObjectTraits<Entity::ActionComponent> : ObjectTraitsBase<Entity::ActionComponent>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<Entity::Component> bases = {};
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::Entity::ActionComponent, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::Entity::ActionComponent, BinaryArchive>
    {
        ObjectT::Activator activator;
    };

    template<>
    class Scribe<::Atmos::Entity::ActionComponent, BinaryArchive> :
        public ObjectScribe<::Atmos::Entity::ActionComponent, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {
        public:
            Table();
        };
    };

    template<>
    class Scribe<::Atmos::Entity::ActionComponent::Activator, BinaryArchive> :
        public EnumScribe<::Atmos::Entity::ActionComponent::Activator, BinaryArchive>
    {};
}