#pragma once

#include "JavaScriptType.h"

namespace Atmos::Scripting::JavaScript
{
    struct GridSpatialNamespace
    {};

    template<>
    class Type<GridSpatialNamespace>
    {
    public:
        [[nodiscard]] static v8::Local<v8::Value> Register(const RegistrationData& data);
    private:
        static void OnDistance(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void OnContains(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void OnIntersects(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void OnToPoint(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void OnToPoint2D(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void OnToPoint3D(const v8::FunctionCallbackInfo<v8::Value>& info);
    };
}