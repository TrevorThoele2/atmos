#pragma once

#include "JavaScriptType.h"

namespace Atmos::Scripting::JavaScript
{
    struct SpatialNamespace
    {};

    template<>
    class Type<SpatialNamespace>
    {
    public:
        [[nodiscard]] static v8::Local<v8::Value> Register(const RegistrationData& data);
        [[nodiscard]] static v8::Local<v8::Value> Traits(const RegistrationData& data);
    private:
        static void OnLeft(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void OnTop(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void OnFarZ(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void OnRight(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void OnBottom(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void OnNearZ(const v8::FunctionCallbackInfo<v8::Value>& info);

        template<class T>
        static void ExtractCoordinate(
            const v8::FunctionCallbackInfo<v8::Value>& info, typename T::Coordinate (T::*extract)() const);
    private:
        static void OnDistance(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void OnIsBetween(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void OnAngleOf(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void OnContains(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void OnIntersects(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void OnEnvelope(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void OnClamp(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void OnScaleOf(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void OnCell(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void OnAdd(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void OnSubtract(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void OnToPoint2D(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void OnToPoint3D(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void OnToDegrees(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void OnToAxisAlignedBox2D(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void OnToAxisAlignedBox3D(const v8::FunctionCallbackInfo<v8::Value>& info);
    };

    template<class T>
	void Type<SpatialNamespace>::ExtractCoordinate(
        const v8::FunctionCallbackInfo<v8::Value>& info, typename T::Coordinate(T::* extract)() const)
    {
        const auto isolate = info.GetIsolate();

        const auto box = FromV8<T>(*isolate, info[0]);
        if (box)
        {
            const auto value = *box;
            const auto coordinate = (value.*extract)();
            info.GetReturnValue().Set(ToV8(*isolate, coordinate));
        }
    }
}