#pragma once

#include <v8.h>

namespace Atmos::Scripting::JavaScript
{
    template<class T>
    using CopyablePersistent = v8::Persistent<T, v8::CopyablePersistentTraits<T>>;
}