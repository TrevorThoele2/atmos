#include "JavaScriptRender.h"

#include "JavaScriptCamera.h"
#include "JavaScriptChangeColor.h"
#include "JavaScriptChangeImageCore.h"
#include "JavaScriptMoveGridRegion.h"
#include "JavaScriptChangeMaterialAsset.h"
#include "JavaScriptChangeTextCore.h"
#include "JavaScriptChangeViewSlice.h"
#include "JavaScriptFindImagesByBox.h"
#include "JavaScriptFindLinesByBox.h"
#include "JavaScriptFindGridRegionsByBox.h"
#include "JavaScriptMoveLine.h"
#include "JavaScriptTextBaseSize.h"
#include "JavaScriptTraits.h"

#include "JavaScriptDynamicImage.h"
#include "JavaScriptDynamicText.h"
#include "JavaScriptGridRegion.h"
#include "JavaScriptImageCore.h"
#include "JavaScriptLine.h"
#include "JavaScriptRenderCore.h"
#include "JavaScriptViewSlice.h"

#include "JavaScriptObject.h"
#include <Arca/Destroy.h>

namespace Atmos::Scripting::JavaScript
{
    v8::Local<v8::Value> Type<RenderNamespace>::Traits(const RegistrationData& data)
    {
        const auto isolate = data.isolate;

        return CreateObject(
            *isolate,
            {
                { "Camera", ToV8(*isolate, ArcaTraits::From<Render::Camera>()) },
                { "ChangeColor", ToV8(*isolate, ArcaTraits::From<Render::ChangeColor>()) },
                { "ChangeImageCore", ToV8(*isolate, ArcaTraits::From<Render::ChangeImageCore>()) },
                { "MoveGridRegion", ToV8(*isolate, ArcaTraits::From<Render::MoveGridRegion>()) },
                { "ChangeMaterialAsset", ToV8(*isolate, ArcaTraits::From<Render::ChangeMaterialAsset>()) },
                { "ChangeTextCore", ToV8(*isolate, ArcaTraits::From<Render::ChangeTextCore>()) },
                { "ChangeViewSlice", ToV8(*isolate, ArcaTraits::From<Render::ChangeViewSlice>()) },
                { "FindImagesByBox", ToV8(*isolate, ArcaTraits::From<Render::FindImagesByBox>()) },
                { "FindLinesByBox", ToV8(*isolate, ArcaTraits::From<Render::FindLinesByBox>()) },
                { "FindGridRegionsByBox", ToV8(*isolate, ArcaTraits::From<Render::FindGridRegionsByBox>()) },
                { "MoveLine", ToV8(*isolate, ArcaTraits::From<Render::MoveLine>()) },
                { "TextBaseSize", ToV8(*isolate, ArcaTraits::From<Render::TextBaseSize>()) },

                { "DynamicImage", ToV8(*isolate, ArcaTraits::From<Render::DynamicImage>()) },
                { "CreateDynamicImage", ToV8(*isolate, ArcaTraits::From<Arca::Create<Render::DynamicImage>>()) },
                { "DestroyDynamicImage", ToV8(*isolate, ArcaTraits::From<Arca::Destroy<Render::DynamicImage>>()) },
                { "DynamicText", ToV8(*isolate, ArcaTraits::From<Render::DynamicText>()) },
                { "CreateDynamicText", ToV8(*isolate, ArcaTraits::From<Arca::Create<Render::DynamicText>>()) },
                { "DestroyDynamicText", ToV8(*isolate, ArcaTraits::From<Arca::Destroy<Render::DynamicText>>()) },
                { "GridRegion", ToV8(*isolate, ArcaTraits::From<Render::GridRegion>()) },
                { "CreateGridRegion", ToV8(*isolate, ArcaTraits::From<Arca::Create<Render::GridRegion>>()) },
                { "DestroyGridRegion", ToV8(*isolate, ArcaTraits::From<Arca::Destroy<Render::GridRegion>>()) },
                { "ImageCore", ToV8(*isolate, ArcaTraits::From<Render::ImageCore>()) },
                { "CreateImageCore", ToV8(*isolate, ArcaTraits::From<Arca::Create<Render::ImageCore>>()) },
                { "DestroyImageCore", ToV8(*isolate, ArcaTraits::From<Arca::Destroy<Render::ImageCore>>()) },
                { "Line", ToV8(*isolate, ArcaTraits::From<Render::Line>()) },
                { "CreateLine", ToV8(*isolate, ArcaTraits::From<Arca::Create<Render::Line>>()) },
                { "DestroyLine", ToV8(*isolate, ArcaTraits::From<Arca::Destroy<Render::Line>>()) },
                { "RenderCore", ToV8(*isolate, ArcaTraits::From<Render::RenderCore>()) },
                { "CreateRenderCore", ToV8(*isolate, ArcaTraits::From<Arca::Create<Render::RenderCore>>()) },
                { "DestroyRenderCore", ToV8(*isolate, ArcaTraits::From<Arca::Destroy<Render::RenderCore>>()) },
                { "ViewSlice", ToV8(*isolate, ArcaTraits::From<Render::ViewSlice>()) },
                { "CreateViewSlice", ToV8(*isolate, ArcaTraits::From<Arca::Create<Render::ViewSlice>>()) },
                { "DestroyViewSlice", ToV8(*isolate, ArcaTraits::From<Arca::Destroy<Render::ViewSlice>>()) }
            });
    }
}