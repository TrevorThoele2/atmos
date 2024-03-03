
#include "InputScribeBase.h"

namespace Atmos
{
    InputScribeBase::~InputScribeBase()
    {}

    InputScribeBase::InputScribeBase(WorldStartProperty::Getter worldStartGetter) : worldStart(worldStartGetter)
    {}
}