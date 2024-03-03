#include "InputAngelScriptBytecodeStream.h"

#include <Chroma/Contract.h>

namespace Atmos::Scripting::Angel
{
    InputBytecodeStream::InputBytecodeStream(const Buffer& buffer) :
        buffer(buffer)
    {}

    int InputBytecodeStream::Write(const void* ptr, asUINT size)
    {
        DEBUG_ASSERT(false);
        return {};
    }

    int InputBytecodeStream::Read(void* ptr, asUINT size)
    {
        if (size > buffer.size())
            return -1;

        try
        {
            memcpy(ptr, &buffer[location], size);
            location += size;

            return 0;
        }
        catch(...)
        {
            return -1;
        }
    }
}