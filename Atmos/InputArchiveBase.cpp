#include "InputArchiveBase.h"

namespace Atmos::World::Serialization
{
    InputArchiveBase::~InputArchiveBase()
    {}

    InputArchiveBase::InputArchiveBase(StartProperty::Getter worldStartGetter) : worldStart(worldStartGetter)
    {}
}