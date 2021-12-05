#include "JavaScriptInput.h"

#include "JavaScriptMouseMoved.h"
#include "JavaScriptTraits.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    v8::Local<v8::Value> Type<InputNamespace>::Traits(const RegistrationData& data)
    {
        const auto isolate = data.isolate;

        return CreateObject(
            *isolate,
            {
                { "MouseMoved", ToV8(*isolate, ArcaTraits::From<Input::MouseMoved>()) }
            });
    }
}