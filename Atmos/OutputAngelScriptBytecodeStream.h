#pragma once

#include <angelscript.h>

#include <Inscription/Buffer.h>

namespace Atmos::Scripting::Angel
{
    class OutputBytecodeStream final : public asIBinaryStream
    {
    public:
        int Write(const void* ptr, asUINT size) override;
        int Read(void* ptr, asUINT size) override;

        [[nodiscard]] Inscription::Buffer Buffer() const;
    private:
        Inscription::Buffer buffer;
        asUINT location = 0;
    };
}