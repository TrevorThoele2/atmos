#include "JavaScriptAsset.h"

#include "JavaScriptActionAsset.h"
#include "JavaScriptAudioAsset.h"
#include "JavaScriptFontAsset.h"
#include "JavaScriptImageAsset.h"
#include "JavaScriptMaterialAsset.h"
#include "JavaScriptScriptAsset.h"
#include "JavaScriptShaderAsset.h"
#include "JavaScriptFindAssetByName.h"
#include "JavaScriptTraits.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    v8::Local<v8::Value> Type<AssetNamespace>::Traits(const RegistrationData& data)
    {
        const auto isolate = data.isolate;

        return CreateObject(
            *isolate,
            {
                { "Action", ToV8(*isolate, ArcaTraits::From<Asset::Action>()) },
                { "Audio", ToV8(*isolate, ArcaTraits::From<Asset::Audio>()) },
                { "Font", ToV8(*isolate, ArcaTraits::From<Asset::Font>()) },
                { "Image", ToV8(*isolate, ArcaTraits::From<Asset::Image>()) },
                { "Material", ToV8(*isolate, ArcaTraits::From<Asset::Material>()) },
                { "Script", ToV8(*isolate, ArcaTraits::From<Asset::Script>()) },
                { "Shader", ToV8(*isolate, ArcaTraits::From<Asset::Shader>()) },
                {
                    "FindByName",
                    CreateObject(
                        *isolate,
                        {
                            { "Action", ToV8(*isolate, ArcaTraits::From<Asset::FindByName<Asset::Action>>()) },
                            { "Audio", ToV8(*isolate, ArcaTraits::From<Asset::FindByName<Asset::Audio>>()) },
                            { "Font", ToV8(*isolate, ArcaTraits::From<Asset::FindByName<Asset::Font>>()) },
                            { "Image", ToV8(*isolate, ArcaTraits::From<Asset::FindByName<Asset::Image>>()) },
                            { "Material", ToV8(*isolate, ArcaTraits::From<Asset::FindByName<Asset::Material>>()) },
                            { "Script", ToV8(*isolate, ArcaTraits::From<Asset::FindByName<Asset::Script>>()) },
                            { "Shader", ToV8(*isolate, ArcaTraits::From<Asset::FindByName<Asset::Shader>>()) },
                        })
                }
            });
    }
}