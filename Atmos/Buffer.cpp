#include "Buffer.h"

namespace Atmos
{
    bool CompareBuffers(const void* first, const void* second, size_t size)
    {
        if (first == second)
            return true;

        if (!first || !second)
            return false;

        const auto useFirst = reinterpret_cast<const char*>(first);
        const auto useSecond = reinterpret_cast<const char*>(second);

        for (size_t i = 0; i != size; ++i)
            if (useFirst[i] != useSecond[i])
                return false;

        return true;
    }
}