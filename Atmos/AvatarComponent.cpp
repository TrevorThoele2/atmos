
#include "AvatarComponent.h"
#include <Inscription\Scribe.h>

namespace Atmos
{
    namespace Ent
    {
        INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(AvatarComponent)
        {
            scribe(gold);
        }

        AvatarComponent::AvatarComponent() : gold(0)
        {}

        AvatarComponent::AvatarComponent(AvatarComponent &&arg) : Component(std::move(arg)), gold(std::move(arg.gold))
        {}

        AvatarComponent& AvatarComponent::operator=(AvatarComponent &&arg)
        {
            Component::operator=(std::move(arg));
            gold = std::move(arg.gold);
            return *this;
        }
    }
}