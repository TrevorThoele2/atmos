#include "JavaScriptUI.h"

#include "JavaScriptUIImage.h"
#include "JavaScriptUIText.h"
#include "JavaScriptTraits.h"

#include "JavaScriptObject.h"
#include <Arca/Destroy.h>

namespace Atmos::Scripting::JavaScript
{
    v8::Local<v8::Value> Type<UINamespace>::Traits(const RegistrationData& data)
    {
        const auto isolate = data.isolate;

        return CreateObject(
            *isolate,
            {
                { "Image", ToV8(*isolate, ArcaTraits::From<UI::Image>()) },
                { "CreateImage", ToV8(*isolate, ArcaTraits::From<Arca::Create<UI::Image>>()) },
                { "DestroyImage", ToV8(*isolate, ArcaTraits::From<Arca::Destroy<UI::Image>>()) },
                { "Text", ToV8(*isolate, ArcaTraits::From<UI::Text>()) },
                { "CreateText", ToV8(*isolate, ArcaTraits::From<Arca::Create<UI::Text>>()) },
                { "DestroyText", ToV8(*isolate, ArcaTraits::From<Arca::Destroy<UI::Text>>()) }
            });
    }
}