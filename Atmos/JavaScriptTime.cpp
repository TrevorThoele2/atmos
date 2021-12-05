#include "JavaScriptTime.h"

#include "JavaScriptTimePoint.h"

#include "JavaScriptNumeric.h"

#include "JavaScriptObject.h"
#include "JavaScriptFunction.h"
#include "JavaScriptUserData.h"
#include <Arca/Destroy.h>
#include "FrameInformation.h"

namespace Atmos::Scripting::JavaScript
{
    v8::Local<v8::Value> Type<TimeNamespace>::Register(const RegistrationData& data)
    {
        const auto isolate = data.isolate;

        return CreateObject(
            *isolate,
            {
                { "currentTime", CreateFunction(*isolate, data.context, &OnCurrentTime) },
                { "currentFrameTime", CreateFunction(*isolate, data.context, &OnCurrentFrameTime) }
            });
    }

    void Type<TimeNamespace>::OnCurrentTime(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        const auto isolate = info.GetIsolate();

        const auto nanoseconds = std::chrono::high_resolution_clock::now();

        info.GetReturnValue().Set(ToV8(*isolate, nanoseconds));
    }

    void Type<TimeNamespace>::OnCurrentFrameTime(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        const auto isolate = info.GetIsolate();
        const auto userData = static_cast<UserData*>(isolate->GetData(0));

        const auto frameInformation = userData->reliquary->Find<Frame::Information>();

        const auto nanoseconds = Time::Point<>() + frameInformation->totalElapsed;

        info.GetReturnValue().Set(ToV8(*isolate, nanoseconds));
    }
}