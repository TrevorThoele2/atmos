
#include "SenseComponent.h"

#include <Inscription/Vector.h>

namespace Atmos
{
    namespace Ent
    {
        nSenseComponent::nSenseComponent(EntityReference reference) : nEntityComponent(reference), enabled(true)
        {}

        nSenseComponent::nSenseComponent(const ::Inscription::Table<nSenseComponent>& table) : INSCRIPTION_TABLE_GET_BASE(nEntityComponent)
        {}

        void nSenseComponent::Enable(bool enable)
        {
            enabled = enable;
            SyncObjects();
        }

        void nSenseComponent::Disable()
        {
            Enable(false);
        }

        bool nSenseComponent::IsEnabled() const
        {
            return enabled;
        }

        void nSenseComponent::SyncObjects()
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

        ObjectTypeDescription nSenseComponent::TypeDescription() const
        {
            return ObjectTraits<nSenseComponent>::TypeDescription();
        }

        INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(nSenseComponent)
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

    const ObjectTypeName ObjectTraits<Ent::nSenseComponent>::typeName = "SenseComponent";
}

namespace Inscription
{
    DEFINE_OBJECT_INSCRIPTER_MEMBERS(::Atmos::Ent::nSenseComponent)
    {
        INSCRIPTION_TABLE_ADD(position);
        INSCRIPTION_TABLE_ADD(sprites);
        INSCRIPTION_TABLE_ADD(sounds);
        INSCRIPTION_TABLE_ADD(enabled);
    }

    INSCRIPTION_INSCRIPTER_DEFINE_SERIALIZE_FUNCTION(::Atmos::Ent::nSenseComponent)
    {
        INSCRIPTION_INSCRIPTER_CALL_BASE_SERIALIZE_FUNCTION;
        if (scribe.IsInput())
            obj.SyncObjects();
    }
}