#pragma once

#include <angelscript.h>

#include "Buffer.h"

namespace Atmos::Scripting::Angel
{
    class OutputBytecodeStream final : public asIBinaryStream
    {
    public:
        int Write(const void* ptr, asUINT size) override;
        int Read(void* ptr, asUINT size) override;

        [[nodiscard]] Buffer Buffer() const;
    private:
        Atmos::Buffer buffer;
        asUINT location = 0;
    };
}