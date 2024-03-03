#pragma once

#include "JavaScriptType.h"
#include "Log.h"

namespace Atmos::Scripting::JavaScript
{
    struct EntityNamespace
    {};

    template<>
    class Type<EntityNamespace>
    {
    public:
        [[nodiscard]] static v8::Local<v8::Value> Register(const RegistrationData& data);
        [[nodiscard]] static v8::Local<v8::Value> Traits(const RegistrationData& data);
    private:
        static void OnCurrentEntity(const v8::FunctionCallbackInfo<v8::Value>& info);
    };
}