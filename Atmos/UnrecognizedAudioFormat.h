#pragma once

#include "Exception.h"
#include "Name.h"

namespace Atmos::Audio
{
    class UnrecognizedAudioFormat final : public Exception
    {
    public:
        explicit UnrecognizedAudioFormat(const Name& name);
    };
}