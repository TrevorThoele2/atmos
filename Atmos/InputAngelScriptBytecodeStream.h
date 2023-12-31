#pragma once

#include <angelscript.h>

#include "Buffer.h"

namespace Atmos::Scripting::Angel
{
    class InputBytecodeStream final : public asIBinaryStream
    {
    public:
        explicit InputBytecodeStream(const Buffer& buffer);

        int Write(const void* ptr, asUINT size) override;
        int Read(void* ptr, asUINT size) override;
    private:
        Buffer buffer;
        asUINT location = 0;
    };
}