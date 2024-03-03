#pragma once

#include <Chroma/Variant.h>
#include "InputKeyID.h"
#include "InputMouseKeyID.h"

namespace Atmos::Input
{
    typedef ::Chroma::Variant<KeyID, MouseKeyID> KeyIDVariant;
}