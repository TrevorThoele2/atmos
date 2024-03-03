#pragma once

#include "JavaScriptType.h"
#include "MaterialAssetPass.h"

#include "JavaScriptShaderAsset.h"
#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    struct MaterialAssetPass
    {
        ShaderAsset vertexShader;
        ShaderAsset fragmentShader;
    };

    template<>
    class Type<MaterialAssetPass>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const MaterialAssetPass& value);
        [[nodiscard]] static std::optional<MaterialAssetPass> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<ShaderAsset> vertexShader;
            Property<ShaderAsset> fragmentShader;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };

    template<>
    class Type<Asset::MaterialPass>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Asset::MaterialPass& value);
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Scripting::JavaScript::MaterialAssetPass> final
    {
    public:
        using ObjectT = Atmos::Scripting::JavaScript::MaterialAssetPass;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            format("vertexShader", object.vertexShader);
            format("fragmentShader", object.fragmentShader);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Scripting::JavaScript::MaterialAssetPass, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::MaterialAssetPass>;
    };
}