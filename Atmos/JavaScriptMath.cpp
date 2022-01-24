#include "JavaScriptMath.h"

#include "JavaScriptNumeric.h"

#include "JavaScriptObject.h"
#include "JavaScriptFunction.h"
#include "JavaScriptUserData.h"

#include "CurrentActualizingEntity.h"

#include <Arca/Destroy.h>

namespace Atmos::Scripting::JavaScript
{
    v8::Local<v8::Value> Type<MathNamespace>::Register(const RegistrationData& data)
    {
        const auto isolate = data.isolate;

        return CreateObject(
            *isolate,
            {
                {
                    "Float",
                    CreateObject(
                        *isolate,
                        {
                            { "maximum", ToV8(*isolate, std::numeric_limits<float>::max()) }
                        })
                }
            });
    }
}