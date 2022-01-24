#include "JavaScriptInput.h"

#include "JavaScriptInputInformation.h"
#include "JavaScriptMouseMoved.h"
#include "JavaScriptActionPressed.h"
#include "JavaScriptActionDepressed.h"
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
                { "Information", ToV8(*isolate, ArcaTraits::From<Input::Information>()) },
                { "MouseMoved", ToV8(*isolate, ArcaTraits::From<Input::MouseMoved>()) },
                { "ActionPressed", ToV8(*isolate, ArcaTraits::From<Input::ActionPressed>()) },
                { "ActionDepressed", ToV8(*isolate, ArcaTraits::From<Input::ActionDepressed>()) }
            });
    }
}