#include "JavaScriptRegisterAll.h"

#include "JavaScriptRegistrationData.h"
#include "JavaScriptSyntheticModule.h"

#include "JavaScriptReliquary.h"
#include "JavaScriptTraits.h"
#include "JavaScriptAsset.h"
#include "JavaScriptInput.h"
#include "JavaScriptSpatial.h"
#include "JavaScriptLogging.h"
#include "JavaScriptEntityNamespace.h"
#include "JavaScriptTime.h"
#include "JavaScriptMath.h"

namespace Atmos::Scripting::JavaScript
{
	v8::MaybeLocal<v8::Value> RegisterAll(v8::Local<v8::Context> context, v8::Local<v8::Module> module)
	{
        const auto isolate = context->GetIsolate();

        try
        {
            const auto data = RegistrationData{ context->GetIsolate(), context, module };
            const auto atmosObject = CreateObject(
                *isolate,
                {
                    { "Traits", Type<ArcaTraits>::Register(data) },
                    { "Spatial", Type<SpatialNamespace>::Register(data) },
                    { "Logging", Type<LoggingNamespace>::Register(data) },
                    { "Reliquary", Type<ReliquaryNamespace>::Register(data) },
                    { "Entity", Type<EntityNamespace>::Register(data) },
                    { "Time", Type<TimeNamespace>::Register(data) },
                    { "Math", Type<MathNamespace>::Register(data) }
                });

            SetSyntheticModuleExport(*isolate, module, "Atmos", atmosObject);

            return {v8::True(isolate)};
        }
        catch (...)
        {
            return {};
        }
	}
}