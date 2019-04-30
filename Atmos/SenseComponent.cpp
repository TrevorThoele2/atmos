
#include "SenseComponent.h"

#include <Inscription/Vector.h>

namespace Atmos
{
    namespace Entity
    {
        SenseComponent::SenseComponent(ObjectManager& manager, EntityReference reference) :
            Component(manager, reference), enabled(true)
        {}

        SenseComponent::SenseComponent(const ::Inscription::Table<SenseComponent>& table) : INSCRIPTION_TABLE_GET_BASE(Component)
        {}

        void SenseComponent::Enable(bool enable)
        {
            enabled = enable;
            SyncObjects();
        }

        void SenseComponent::Disable()
        {
            Enable(false);
        }

        bool SenseComponent::IsEnabled() const
        {
            return enabled;
        }

        void SenseComponent::SyncObjects()
        {
            if (enabled)
            {
                for (auto& loop : sprites)
                {
                    //GetCurrentRenderFragments()->Add(*loop.source);
                    loop.source->enabled = true;
                }

                for (auto& loop : sounds)
                {
                    loop.source->enabled = true;
                }
            }
            else
            {
                for (auto& loop : sprites)
                {
                    //GetCurrentRenderFragments()->Remove(*loop.source);
                    loop.source->enabled = false;
                }

                for (auto& loop : sounds)
                {
                    loop.source->enabled = false;
                }
            }
        }

        ObjectTypeDescription SenseComponent::TypeDescription() const
        {
            return ObjectTraits<SenseComponent>::TypeDescription();
        }

        INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(SenseComponent)
        {
            if (scribe.IsOutput())
            {
                scribe.Save(position);
                scribe.Save(sprites);
            }
            else // INPUT
            {
                scribe.Load(position);
                scribe.Load(sprites);

                SyncObjects();
            }
        }
    }

    const ObjectTypeName ObjectTraits<Entity::SenseComponent>::typeName = "SenseComponent";
}

namespace Inscription
{
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::Entity::SenseComponent)
    {
        INSCRIPTION_TABLE_ADD(position);
        INSCRIPTION_TABLE_ADD(sprites);
        INSCRIPTION_TABLE_ADD(sounds);
        INSCRIPTION_TABLE_ADD(enabled);
    }

    INSCRIPTION_INSCRIPTER_DEFINE_SERIALIZE_FUNCTION(::Atmos::Entity::SenseComponent)
    {
        INSCRIPTION_INSCRIPTER_CALL_BASE_SERIALIZE_FUNCTION;
        if (scribe.IsInput())
            obj.SyncObjects();
    }
}