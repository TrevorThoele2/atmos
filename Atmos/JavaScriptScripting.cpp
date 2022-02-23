#include "JavaScriptScripting.h"

#include "JavaScriptScript.h"
#include "JavaScriptTraits.h"

#include "JavaScriptObject.h"
#include "JavaScriptFunction.h"
#include "JavaScriptUserData.h"

#include <Arca/Destroy.h>

namespace Atmos::Scripting::JavaScript
{
    v8::Local<v8::Value> Type<ScriptingNamespace>::Traits(const RegistrationData& data)
    {
        const auto isolate = data.isolate;

        return CreateObject(
            *isolate,
            {
                { "Script", ToV8(*isolate, ArcaTraits::From<Scripting::Script>()) },
                { "CreateScript", ToV8(*isolate, ArcaTraits::From<Arca::Create<Scripting::Script>>()) },
                { "DestroyScript", ToV8(*isolate, ArcaTraits::From<Arca::Destroy<Scripting::Script>>()) }
            });
    }
}
