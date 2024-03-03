#pragma once

#include <vector>

namespace Atmos
{
    using Buffer = std::vector<char>;
    using UnsignedBuffer = std::vector<unsigned char>;

    template<class T, class PositionT>
    size_t ReadFromBuffer(const char* buffer, T& obj, PositionT position)
    {
        memcpy(&obj, &reinterpret_cast<const char*>(buffer)[position], sizeof(T));
        return sizeof(T);
    }

    template<class PositionT>
    size_t CopyBuffer(const void* from, void* to, size_t size, PositionT position)
    {
        memcpy(to, &reinterpret_cast<const char*>(from)[position], size);
        return size;
    }

    bool CompareBuffers(const void* first, const void* second, size_t size);

    UnsignedBuffer ToUnsignedBuffer(const Buffer& buffer);
    Buffer ToBuffer(const UnsignedBuffer& unsignedBuffer);
}