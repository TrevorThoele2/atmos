#pragma once

#include "nEntityComponent.h"

#include "ScriptInstance.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    namespace Ent
    {
        class nActionComponent : public nEntityComponent
        {
        public:
            enum class Activator : std::int32_t
            {
                ENTER_TILE,
                ATTEMPT_ENTER_TILE,
                USE_ON,
                USE_INTO,
                ENTER_FIELD,
                LEAVE_FIELD
            };
        public:
            typedef TypedObjectReference<ScriptInstance> ScriptInstanceReference;
        public:
            Activator activator;
            ScriptInstanceReference script;

            nActionComponent(EntityReference owner);
            nActionComponent(const nActionComponent& arg) = default;
            nActionComponent(const ::Inscription::Table<nActionComponent>& table);

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

    template<>
    struct ObjectTraits<Ent::nActionComponent> : ObjectTraitsBase<Ent::nActionComponent>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::Ent::nActionComponent)
    {
    public:
        static void AddMembers(TableT& table);
    };
}