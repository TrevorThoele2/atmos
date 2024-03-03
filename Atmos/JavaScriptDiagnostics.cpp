#include "JavaScriptDiagnostics.h"

#include "JavaScriptDiagnosticsStatistics.h"

#include "JavaScriptTraits.h"

#include <Arca/Destroy.h>

namespace Atmos::Scripting::JavaScript
{
    v8::Local<v8::Value> Type<DiagnosticsNamespace>::Traits(const RegistrationData& data)
    {
        const auto isolate = data.isolate;

        return CreateObject(
            *isolate,
            {
                { "Statistics", ToV8(*isolate, ArcaTraits::From<Diagnostics::Statistics>()) },
            });
    }
}