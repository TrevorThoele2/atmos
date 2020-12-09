#include "OutputAngelScriptBytecodeStream.h"

#include <Chroma/Contract.h>

#include <Inscription/BufferScribe.h>

namespace Atmos::Scripting::Angel
{
    int OutputBytecodeStream::Write(const void* ptr, asUINT size)
    {
        if (size == 0)
            return -1;

        try
        {
            const auto castedPtr = reinterpret_cast<const char*>(ptr);

            buffer.resize(buffer.size() + size);
            memcpy(&buffer[location], castedPtr, size);
            location += size;

            return 0;
        }
        catch(...)
        {
            return -1;
        }
    }

    int OutputBytecodeStream::Read(void* ptr, asUINT size)
    {
        DEBUG_ASSERT(false);
        return {};
    }

    Inscription::Buffer OutputBytecodeStream::Buffer() const
    {
        return buffer;
    }
}