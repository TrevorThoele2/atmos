
#include "Buffer.h"

namespace Atmos
{
    bool CompareBuffers(const void *first, const void *second, size_t size)
    {
        const char *useFirst = reinterpret_cast<const char*>(first);
        const char *useSecond = reinterpret_cast<const char*>(second);

        for (size_t loop = 0; loop != size; ++loop)
        {
            if (useFirst[loop] != useSecond[loop])
                return false;
        }

        return true;
    }
}