#pragma once

#include "JavaScriptType.h"

namespace Atmos::Scripting::JavaScript
{
    struct AssetNamespace
    {};

    template<>
    class Type<AssetNamespace>
    {
    public:
        [[nodiscard]] static v8::Local<v8::Value> Traits(const RegistrationData& data);
    };
}