#include "JavaScriptTraits.h"

#include "JavaScriptString.h"
#include "JavaScriptAsset.h"
#include "JavaScriptAudio.h"
#include "JavaScriptInput.h"
#include "JavaScriptLogging.h"
#include "JavaScriptDiagnostics.h"
#include "JavaScriptRender.h"
#include "JavaScriptSpatial.h"
#include "JavaScriptEntityNamespace.h"
#include "JavaScriptScripting.h"
#include "JavaScriptUI.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
	v8::Local<v8::Value> Type<ArcaTraits>::Register(const RegistrationData& data)
    {
        const auto isolate = data.isolate;

        return CreateObject(
            *isolate,
            {
                { "Asset", Type<AssetNamespace>::Traits(data) },
                { "Audio", Type<AudioNamespace>::Traits(data) },
                { "Entity", Type<EntityNamespace>::Traits(data) },
                { "Input", Type<InputNamespace>::Traits(data) },
                { "Logging", Type<LoggingNamespace>::Traits(data) },
                { "Diagnostics", Type<DiagnosticsNamespace>::Traits(data) },
                { "Render", Type<RenderNamespace>::Traits(data) },
                { "Spatial", Type<SpatialNamespace>::Traits(data) },
                { "Scripting", Type<ScriptingNamespace>::Traits(data) },
                { "UI", Type<UINamespace>::Traits(data) }
            });
    }

    auto Type<ArcaTraits>::ToV8(v8::Isolate& isolate, const ArcaTraits& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.typeName.SetValue(isolate, value.typeName);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<ArcaTraits> Type<ArcaTraits>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<ArcaTraits, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto typeName = definition.typeName.AtmosValue();
                return typeName
                    ? ArcaTraits
	                {
	                    *typeName
	                }
	                : std::optional<ArcaTraits>{};
            });
    }

    Type<ArcaTraits>::Definition::Definition() :
        typeName("typeName")
    {}

    AnyObjectDefinition Type<ArcaTraits>::Definition::ToAny()
    {
        return
        {
            {
                { typeName }
            }
        };
    }
}