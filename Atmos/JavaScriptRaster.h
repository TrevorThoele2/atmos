#pragma once

#include "JavaScriptType.h"

namespace Atmos::Scripting::JavaScript
{
    struct RasterNamespace
    {};

    template<>
    class Type<RasterNamespace>
    {
    public:
        [[nodiscard]] static v8::Local<v8::Value> Traits(const RegistrationData& data);
    };
}