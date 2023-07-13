#include "JavaScriptShaders.h"

#include "JavaScriptObject.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Render::Shaders>::ToV8(v8::Isolate& isolate, const Render::Shaders& value) -> v8::Local<V8T>
    {
        Definition definition;
        const auto vertex = value.vertex ? ShaderAsset{ value.vertex.ID(), value.vertex->Name() } : std::optional<ShaderAsset>();
        const auto fragment = value.fragment ? ShaderAsset{ value.fragment.ID(), value.fragment->Name() } : std::optional<ShaderAsset>();
        definition.vertex.SetValue(isolate, vertex);
        definition.fragment.SetValue(isolate, fragment);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Render::Shaders> Type<Render::Shaders>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        const auto userData = static_cast<UserData*>(isolate.GetData(0));
        const auto reliquary = userData->reliquary;

        return FromObject<Render::Shaders, Definition>(
            isolate,
            value,
            [&reliquary](const Definition& definition)
            {
                const auto vertex = definition.vertex.AtmosValue();
                const auto fragment = definition.fragment.AtmosValue();
                return vertex && fragment
                    ? Render::Shaders
                    {
                        .vertex = *vertex ? reliquary->Find<Asset::Shader>((*vertex)->id) : Arca::Index<Asset::Shader>(),
                        .fragment = *fragment ? reliquary->Find<Asset::Shader>((*fragment)->id) : Arca::Index<Asset::Shader>()
                    }
                    : std::optional<Render::Shaders>{};
            });
    }

    Type<Render::Shaders>::Definition::Definition() :
        vertex("vertex"),
        fragment("fragment")
    {}

    AnyObjectDefinition Type<Render::Shaders>::Definition::ToAny()
    {
        return
        {
            {
                { vertex },
                { fragment }
            }
        };
    }
}