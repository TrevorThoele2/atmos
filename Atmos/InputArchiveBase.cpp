#include "InputArchiveBase.h"

namespace Atmos
{
    InputArchiveBase::~InputArchiveBase()
    {}

    InputArchiveBase::InputArchiveBase(WorldStartProperty::Getter worldStartGetter) : worldStart(worldStartGetter)
    {}
}