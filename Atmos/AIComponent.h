#pragma once

#include "nEntityComponent.h"

#include "ScriptInstance.h"

namespace Atmos
{
    namespace Ent
    {
        class nAIComponent : public nEntityComponent
        {
        public:
            typedef TypedObjectReference<ScriptInstance> ScriptInstance;
            ScriptInstance script;
        public:
            nAIComponent(EntityReference owner);
            nAIComponent(const nAIComponent& arg) = default;
            nAIComponent(const ::Inscription::Table<nAIComponent>& table);

            ObjectTypeDescription TypeDescription() const override;
        private:
            void SetupScripts();
        private:
            INSCRIPTION_ACCESS;
        };
    }

    template<>
    struct ObjectTraits<Ent::nAIComponent> : ObjectTraitsBase<Ent::nAIComponent>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::Ent::nAIComponent)
    {
    public:
        static void AddMembers(TableT& table);
    };
}