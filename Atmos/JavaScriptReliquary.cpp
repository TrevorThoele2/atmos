#include "JavaScriptReliquary.h"

#include "JavaScriptBool.h"
#include "JavaScriptSet.h"
#include "JavaScriptVector.h"
#include "JavaScriptPoint2D.h"
#include "JavaScriptPoint3D.h"
#include "JavaScriptSize2D.h"
#include "JavaScriptGridPoint.h"
#include "JavaScriptLog.h"
#include "JavaScriptInputInformation.h"
#include "JavaScriptMouseMoved.h"
#include "JavaScriptActionPressed.h"
#include "JavaScriptActionDepressed.h"
#include "JavaScriptActionAsset.h"
#include "JavaScriptAudioAsset.h"
#include "JavaScriptFontAsset.h"
#include "JavaScriptImageAsset.h"
#include "JavaScriptMaterialAsset.h"
#include "JavaScriptScriptAsset.h"
#include "JavaScriptShaderAsset.h"
#include "JavaScriptFindAssetByName.h"
#include "JavaScriptBounds.h"
#include "JavaScriptDestroy.h"
#include "JavaScriptMoveBounds.h"
#include "JavaScriptRotateBounds.h"
#include "JavaScriptScaleBounds.h"
#include "JavaScriptToWorldPoint2D.h"
#include "JavaScriptToWorldPoint3D.h"
#include "JavaScriptChangeSoundVolume.h"
#include "JavaScriptPauseSound.h"
#include "JavaScriptPositionedSound.h"
#include "JavaScriptResumeSound.h"
#include "JavaScriptSoundFinished.h"
#include "JavaScriptUniversalSound.h"
#include "JavaScriptDiagnosticsStatistics.h"
#include "JavaScriptEntity.h"
#include "JavaScriptFindEntityByName.h"
#include "JavaScriptFindEntityByPosition.h"
#include "JavaScriptFindEntityPath.h"
#include "JavaScriptModifyEntityTags.h"
#include "JavaScriptMoveEntityTo.h"
#include "JavaScriptCanMoveEntityTo.h"
#include "JavaScriptRequestField.h"
#include "JavaScriptUIImage.h"
#include "JavaScriptUIText.h"
#include "JavaScriptCamera.h"
#include "JavaScriptChangeColor.h"
#include "JavaScriptChangeImageCore.h"
#include "JavaScriptMoveGridRegion.h"
#include "JavaScriptChangeMaterialAsset.h"
#include "JavaScriptChangeTextCore.h"
#include "JavaScriptChangeViewSlice.h"
#include "JavaScriptDynamicImage.h"
#include "JavaScriptDynamicText.h"
#include "JavaScriptFindImagesByBox.h"
#include "JavaScriptFindLinesByBox.h"
#include "JavaScriptFindGridRegionsByBox.h"
#include "JavaScriptGridRegion.h"
#include "JavaScriptImageCore.h"
#include "JavaScriptLine.h"
#include "JavaScriptMoveLine.h"
#include "JavaScriptRenderCore.h"
#include "JavaScriptTextBaseSize.h"
#include "JavaScriptViewSlice.h"
#include "FindAssetByName.h"

#include "JavaScriptObject.h"
#include "JavaScriptNumeric.h"
#include "JavaScriptFunction.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
    v8::Local<v8::Value> Type<ReliquaryNamespace>::Register(const RegistrationData& data)
    {
        const auto isolate = data.isolate;
        const auto userData = static_cast<UserData*>(isolate->GetData(0));
        
        RegisterCommand<Logging::Log>(*userData);
        RegisterCommand<Spatial::MoveBounds>(*userData);
        RegisterCommand<Spatial::RotateBounds>(*userData);
        RegisterCommand<Spatial::ScaleBounds>(*userData);
        RegisterCommand<Spatial::ToWorldPoint2D>(*userData);
        RegisterCommand<Spatial::ToWorldPoint3D>(*userData);
        RegisterCommand<Asset::FindByName<Asset::Action>>(*userData);
        RegisterCommand<Asset::FindByName<Asset::Audio>>(*userData);
        RegisterCommand<Asset::FindByName<Asset::Font>>(*userData);
        RegisterCommand<Asset::FindByName<Asset::Image>>(*userData);
        RegisterCommand<Asset::FindByName<Asset::Material>>(*userData);
        RegisterCommand<Asset::FindByName<Asset::Script>>(*userData);
        RegisterCommand<Asset::FindByName<Asset::Shader>>(*userData);
        RegisterCommand<Audio::ChangeSoundVolume>(*userData);
        RegisterCommand<Audio::PauseSound>(*userData);
        RegisterCommand<Audio::ResumeSound>(*userData);
        RegisterCommand<Atmos::Entity::CanMoveTo>(*userData);
        RegisterCommand<Atmos::Entity::FindByName>(*userData);
        RegisterCommand<Atmos::Entity::FindByPosition>(*userData);
        RegisterCommand<Atmos::Entity::FindPath>(*userData);
        RegisterCommand<Atmos::Entity::ModifyTags>(*userData);
        RegisterCommand<Atmos::Entity::MoveTo>(*userData);
        RegisterCommand<Atmos::World::RequestField>(*userData);
        RegisterCommand<Atmos::Render::ChangeColor>(*userData);
        RegisterCommand<Atmos::Render::ChangeImageCore>(*userData);
        RegisterCommand<Atmos::Render::MoveGridRegion>(*userData);
        RegisterCommand<Atmos::Render::ChangeMaterialAsset>(*userData);
        RegisterCommand<Atmos::Render::ChangeTextCore>(*userData);
        RegisterCommand<Atmos::Render::ChangeViewSlice>(*userData);
        RegisterCommand<Atmos::Render::FindImagesByBox>(*userData);
        RegisterCommand<Atmos::Render::FindLinesByBox>(*userData);
        RegisterCommand<Atmos::Render::FindGridRegionsByBox>(*userData);
        RegisterCommand<Atmos::Render::MoveLine>(*userData);
        RegisterCommand<Atmos::Render::TextBaseSize>(*userData);

        RegisterSignal<Input::MouseMoved>(*userData);
        RegisterSignal<Input::ActionPressed>(*userData);
        RegisterSignal<Input::ActionDepressed>(*userData);
        RegisterSignal<Audio::SoundFinished>(*userData);

        RegisterFind<Asset::Action>(*userData);
        RegisterFind<Asset::Audio>(*userData);
        RegisterFind<Asset::Font>(*userData);
        RegisterFind<Asset::Image>(*userData);
        RegisterFind<Asset::Material>(*userData);
        RegisterFind<Asset::Script>(*userData);
        RegisterFind<Asset::Shader>(*userData);

        RegisterFind<Spatial::Bounds>(*userData);
        RegisterCommand<Arca::Create<Spatial::Bounds>>(*userData);
        RegisterCommand<Arca::Destroy<Spatial::Bounds>>(*userData);
        RegisterFind<Audio::PositionedSound>(*userData);
        RegisterCommand<Arca::Create<Audio::PositionedSound>>(*userData);
        RegisterCommand<Arca::Destroy<Audio::PositionedSound>>(*userData);
        RegisterFind<Audio::UniversalSound>(*userData);
        RegisterCommand<Arca::Create<Audio::UniversalSound>>(*userData);
        RegisterCommand<Arca::Destroy<Audio::UniversalSound>>(*userData);
        RegisterFind<Atmos::Entity::Entity>(*userData);
        RegisterCommand<Arca::Create<Atmos::Entity::Entity>>(*userData);
        RegisterCommand<Arca::Destroy<Atmos::Entity::Entity>>(*userData);
        RegisterFind<Atmos::UI::Image>(*userData);
        RegisterCommand<Arca::Create<Atmos::UI::Image>>(*userData);
        RegisterCommand<Arca::Destroy<Atmos::UI::Image>>(*userData);
        RegisterFind<Atmos::UI::Text>(*userData);
        RegisterCommand<Arca::Create<Atmos::UI::Text>>(*userData);
        RegisterCommand<Arca::Destroy<Atmos::UI::Text>>(*userData);
        RegisterFind<Atmos::Render::DynamicImage>(*userData);
        RegisterCommand<Arca::Create<Atmos::Render::DynamicImage>>(*userData);
        RegisterCommand<Arca::Destroy<Atmos::Render::DynamicImage>>(*userData);
        RegisterFind<Atmos::Render::DynamicText>(*userData);
        RegisterCommand<Arca::Create<Atmos::Render::DynamicText>>(*userData);
        RegisterCommand<Arca::Destroy<Atmos::Render::DynamicText>>(*userData);
        RegisterFind<Atmos::Render::GridRegion>(*userData);
        RegisterCommand<Arca::Create<Atmos::Render::GridRegion>>(*userData);
        RegisterCommand<Arca::Destroy<Atmos::Render::GridRegion>>(*userData);
        RegisterFind<Atmos::Render::ImageCore>(*userData);
        RegisterCommand<Arca::Create<Atmos::Render::ImageCore>>(*userData);
        RegisterCommand<Arca::Destroy<Atmos::Render::ImageCore>>(*userData);
        RegisterFind<Atmos::Render::Line>(*userData);
        RegisterCommand<Arca::Create<Atmos::Render::Line>>(*userData);
        RegisterCommand<Arca::Destroy<Atmos::Render::Line>>(*userData);
        RegisterFind<Atmos::Render::RenderCore>(*userData);
        RegisterCommand<Arca::Create<Atmos::Render::RenderCore>>(*userData);
        RegisterCommand<Arca::Destroy<Atmos::Render::RenderCore>>(*userData);
        RegisterFind<Atmos::Render::ViewSlice>(*userData);
        RegisterCommand<Arca::Create<Atmos::Render::ViewSlice>>(*userData);
        RegisterCommand<Arca::Destroy<Atmos::Render::ViewSlice>>(*userData);

        RegisterFind<Diagnostics::Statistics>(*userData);
        RegisterFind<Render::Camera>(*userData);
        RegisterFind<Input::Information>(*userData);

        return CreateObject(
            *isolate,
            {
                { "send", CreateFunction(*isolate, data.context, &OnSend) },
                { "listen", CreateFunction(*isolate, data.context, &OnListen) },
                { "find", CreateFunction(*isolate, data.context, &OnFind) }
            });
    }

	void Type<ReliquaryNamespace>::OnSend(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        const auto isolate = info.GetIsolate();
        const auto context = isolate->GetCurrentContext();
        const auto userData = static_cast<UserData*>(isolate->GetData(0));
        
        const auto typeName = FromV8<String>(*isolate, info[0]);
        if (!typeName)
        {
            userData->reliquary->Do(Logging::Log("Unknown type name", Logging::Severity::Error));
            return;
        }

        if (!info[1]->IsObject())
        {
            userData->reliquary->Do(Logging::Log(
                "Command was not an object",
                Logging::Severity::Error,
                {
                    { "TypeName", *typeName }
                }));
            return;
        }

        auto command = info[1]->ToObject(context).ToLocalChecked();
        
        const auto sendFunction = userData->sendFunctions.find(*typeName);
        if (sendFunction == userData->sendFunctions.end())
        {
            userData->reliquary->Do(Logging::Log(
                "Type name was not a command",
                Logging::Severity::Error,
                {
                    { "TypeName", *typeName }
                }));
            return;
        }

        sendFunction->second(*isolate, command, info.GetReturnValue(), *userData->reliquary);
    }

	void Type<ReliquaryNamespace>::OnListen(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        const auto isolate = info.GetIsolate();
        const auto userData = static_cast<UserData*>(isolate->GetData(0));

        const auto typeName = FromV8<String>(*isolate, info[0]);
        if (!typeName)
            return;

        const auto secondArgument = info[1];
        if (secondArgument.IsEmpty() || !secondArgument->IsFunction())
            return;

        const auto localFunction = secondArgument.As<v8::Function>();

        const auto localContext = isolate->GetCurrentContext();

        const auto listenFunctions = userData->listenFunctions.find(*typeName);
        if (listenFunctions == userData->listenFunctions.end())
        {
            userData->reliquary->Do(Logging::Log(
                "Unknown type name",
                Logging::Severity::Error,
                {
                    { "TypeName", *typeName }
                }));
            return;
        }

        for(auto& contextualized : listenFunctions->second)
        {
	        if (contextualized.context == localContext)
	        {
                contextualized.functions.emplace_back(CopyablePersistent<v8::Function>(isolate, localFunction));
                return;
	        }
        }

        const auto context = CopyablePersistent<v8::Context>(isolate, localContext);
        const auto function = CopyablePersistent<v8::Function>(isolate, localFunction);
        const auto functions = std::vector { function };

        listenFunctions->second.emplace_back(context, functions);
    }

	void Type<ReliquaryNamespace>::OnFind(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        const auto isolate = info.GetIsolate();
        const auto userData = static_cast<UserData*>(isolate->GetData(0));

        const auto typeName = FromV8<String>(*isolate, info[0]);
        if (!typeName)
        {
            info.GetReturnValue().SetNull();
            return;
        }

        const auto findFunction = userData->findFunctions.find(*typeName);
        if (findFunction == userData->findFunctions.end())
        {
            userData->reliquary->Do(Logging::Log(
				"Unknown type name",
                Logging::Severity::Error,
                {
                    { "TypeName", *typeName }
                }));
            info.GetReturnValue().SetNull();
            return;
        }

        const auto returnValue = findFunction->second(*isolate, info, *userData->reliquary);
        info.GetReturnValue().Set(returnValue);
    }
}