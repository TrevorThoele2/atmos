#pragma once

#include <angelscript.h>

namespace Atmos::Scripting::Angel
{
    template<class T, class... Args>
    T* GenerateReference(Args&& ... args)
    {
        const auto memory = asAllocMem(sizeof(T));
        new(memory) T{ std::forward<Args>(args)... };
        return reinterpret_cast<T*>(memory);
    }
}