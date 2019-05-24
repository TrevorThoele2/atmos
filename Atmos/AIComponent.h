#pragma once

#include "EntityComponent.h"

#include "ScriptInstance.h"

namespace Atmos
{
    namespace Entity
    {
        class AIComponent : public Component
        {
        public:
            typedef TypedObjectReference<ScriptInstance> ScriptInstance;
            ScriptInstance script;
        public:
            AIComponent(ObjectManager& manager, EntityReference owner);
            AIComponent(const AIComponent& arg) = default;
            INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(AIComponent);

            ObjectTypeDescription TypeDescription() const override;
        private:
            void SetupScripts();
        private:
            INSCRIPTION_ACCESS;
        };
    }

    template<>
    struct ObjectTraits<Entity::AIComponent> : ObjectTraitsBase<Entity::AIComponent>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::Entity::AIComponent)
    {
    public:
        OBJECT_INSCRIPTER_DECLARE_MEMBERS;
    };
}