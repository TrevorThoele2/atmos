#pragma once

#include "JavaScriptType.h"

#include "JavaScriptUserData.h"
#include "JavaScriptMessage.h"
#include "JavaScriptType.h"

namespace Atmos::Scripting::JavaScript
{
    struct ReliquaryNamespace
    {};

    template<>
    class Type<ReliquaryNamespace>
    {
    public:
        [[nodiscard]] static v8::Local<v8::Value> Register(const RegistrationData& data);
    private:
        template<class T>
        static void RegisterCommand(UserData& userData);
        template<class T>
        static void RegisterSignal(UserData& userData);
        template<class T>
        static void RegisterFind(UserData& userData);

        static void OnSend(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void OnListen(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void OnFind(const v8::FunctionCallbackInfo<v8::Value>& info);
    };

    template<class T>
    void Type<ReliquaryNamespace>::RegisterCommand(UserData& userData)
    {
        userData.sendFunctions.emplace(
            Arca::Traits<T>::typeName,
            [](v8::Isolate& isolate, const v8::Local<v8::Value>& value, v8::ReturnValue<v8::Value> returnValue, Arca::Reliquary& reliquary)
            {
                const auto command = FromV8<T>(isolate, value);
                if (command)
                {
                    if constexpr (!Arca::has_command_result_v<T>)
                        reliquary.Do(*command);
                    else
                    {
                        auto result = reliquary.Do(*command);
                        auto javaScriptResult = JavaScript::ToV8(isolate, result);
                        returnValue.Set(javaScriptResult);
                    }
                }
            });
    }

    template<class T>
	void Type<ReliquaryNamespace>::RegisterSignal(UserData& userData)
    {
        userData.listenFunctions.emplace(Arca::Traits<T>::typeName, std::vector<UserData::ContextualizedFunction>{});

        userData.reliquary->On<T>([&userData](const T& signal)
			{
                const auto isolate = userData.isolate;

                v8::Isolate::Scope isolateScope(isolate);
                v8::HandleScope handleScope(isolate);
                const v8::TryCatch tryCatch(isolate);

                const auto listenFunctions = userData.listenFunctions.find(Arca::Traits<T>::typeName);
                for (auto& contextualized : listenFunctions->second)
                {
                    const auto context = contextualized.context.Get(isolate);
                    v8::Context::Scope contextScope(context);

                    std::vector<v8::Local<v8::Value>> parameters = { JavaScript::ToV8(*isolate, signal) };

                    for (auto& function : contextualized.functions)
                    {
                        const auto localFunction = function.Get(isolate);
                        localFunction->Call(context, v8::Null(isolate), 1, parameters.data());

                        if (tryCatch.HasCaught())
                        {
                            const auto failure = ToFailure(*isolate, context, tryCatch.Message());
                            userData.reliquary->Do(ToLog(failure));
                        }
                    }
                }
			});
    }

    template<class T>
    void Type<ReliquaryNamespace>::RegisterFind(UserData& userData)
    {
        userData.findFunctions.emplace(
            Arca::Traits<T>::typeName,
            [](v8::Isolate& isolate, const v8::FunctionCallbackInfo<v8::Value>& info, Arca::Reliquary& reliquary) -> v8::Local<v8::Value>
            {
                if constexpr (Arca::is_local_v<T>)
                {
                    const auto relicID = FromV8<Arca::RelicID>(isolate, info[1]);
                    if (!relicID)
                        info.GetReturnValue().SetNull();
                    else
                    {
                        const auto found = reliquary.Find<T>(*relicID);
                        return found ? JavaScript::ToV8(isolate, found).template As<v8::Value>() : v8::Null(&isolate).As<v8::Value>();
                    }
                }
                else
                {
                    const auto found = reliquary.Find<T>();
                    return found ? JavaScript::ToV8(isolate, found).template As<v8::Value>() : v8::Null(&isolate).As<v8::Value>();
                }

                return v8::Null(&isolate).As<v8::Value>();
            });
    }
}