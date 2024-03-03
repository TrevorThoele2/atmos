#include "Buffer.h"

namespace Atmos
{
    bool CompareBuffers(const void* first, const void* second, size_t size)
    {
        if (first == second)
            return true;

        if (!first || !second)
            return false;

        const auto useFirst = static_cast<const char*>(first);
        const auto useSecond = static_cast<const char*>(second);

        for (size_t i = 0; i != size; ++i)
            if (useFirst[i] != useSecond[i])
                return false;

        return true;
    }

    UnsignedBuffer ToUnsignedBuffer(const Buffer& buffer)
    {
        UnsignedBuffer returnValue;
        returnValue.reserve(static_cast<size_t>(buffer.size()));
        for (auto& byte : buffer)
            returnValue.push_back(*reinterpret_cast<const unsigned char*>(&byte));
        return returnValue;
    }

    Buffer ToBuffer(const UnsignedBuffer& unsignedBuffer)
    {
        Buffer returnValue;
        returnValue.reserve(static_cast<size_t>(unsignedBuffer.size()));
        for (auto& byte : unsignedBuffer)
            returnValue.push_back(*reinterpret_cast<const char*>(&byte));
        return returnValue;
    }
}