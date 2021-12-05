#pragma once

#include "JavaScriptType.h"

namespace Atmos::Scripting::JavaScript
{
    struct TimeNamespace
    {};

    template<>
    class Type<TimeNamespace>
    {
    public:
        [[nodiscard]] static v8::Local<v8::Value> Register(const RegistrationData& data);
        [[nodiscard]] static v8::Local<v8::Value> Traits(const RegistrationData& data);
    private:
        static void OnCurrentTime(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void OnCurrentFrameTime(const v8::FunctionCallbackInfo<v8::Value>& info);
    };
}