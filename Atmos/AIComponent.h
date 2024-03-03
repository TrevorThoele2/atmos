#pragma once

#include "EntityComponent.h"

#include "ScriptInstance.h"

namespace Atmos::Entity
{
    class AIComponent : public Component
    {
    public:
        typedef TypedObjectReference<ScriptInstance> ScriptInstance;
        ScriptInstance script;
    public:
        AIComponent(ObjectManager& manager, EntityReference owner);
        AIComponent(const AIComponent& arg) = default;
        AIComponent(const ::Inscription::BinaryTableData<AIComponent>& data);

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
    struct ObjectTraits<Entity::AIComponent> : ObjectTraitsBase<Entity::AIComponent>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<Entity::Component> bases = {};
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::Entity::AIComponent, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::Entity::AIComponent, BinaryArchive>
    {
        ObjectT::ScriptInstance script;
    };

    template<>
    class Scribe<::Atmos::Entity::AIComponent, BinaryArchive> :
        public ObjectScribe<::Atmos::Entity::AIComponent, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {
        public:
            Table();
        };
    };
}