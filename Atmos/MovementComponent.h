#pragma once

#include "ObjectReference.h"

#include "nEntityComponent.h"

#include "ScriptInstance.h"

namespace Atmos
{
    namespace Ent
    {
        class nMovementComponent : public nEntityComponent
        {
        public:
            typedef TypedObjectReference<ScriptInstance> ScriptInstance;
        public:
            bool enabled;
        public:
            ScriptInstance movementModulatorCreator;
            ScriptInstance changeDirectionModulatorCreator;
        public:
            nMovementComponent(EntityReference reference);
            nMovementComponent(const nMovementComponent& arg) = default;
            nMovementComponent(const ::Inscription::Table<nMovementComponent>& table);

            void Enable();
            void Disable();

            bool IsMoving() const;
            bool CanMove() const;

            ObjectTypeDescription TypeDescription() const override;
        private:
            void SetupScripts();
        private:
            INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        };
    }

    template<>
    struct ObjectTraits<Ent::nMovementComponent> : ObjectTraitsBase<Ent::nMovementComponent>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::Ent::nMovementComponent)
    {
    public:
        static void AddMembers(TableT& table);
    };
}