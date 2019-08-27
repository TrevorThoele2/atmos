#include "Action.h"

namespace Atmos::Input
{
    Action::Action(String displayName) : displayName(std::move(displayName))
    {}
}