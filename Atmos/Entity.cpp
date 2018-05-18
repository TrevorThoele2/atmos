
#include "Entity.h"
#include <Inscription/Scribe.h>

namespace Atmos
{
    namespace Ent
    {
        INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(Entity)
        {
            scribe(val);
        }

        Entity::Entity(ValueT val) : val(val)
        {}

        Entity& Entity::operator=(ValueT val)
        {
            this->val = val;
            return *this;
        }

        Entity::operator ValueT() const
        {
            return val;
        }

        Entity::ValueT Entity::operator*() const
        {
            return val;
        }
    }
}