#include "JavaScriptUIText.h"

#include "JavaScriptBool.h"
#include "JavaScriptNumeric.h"
#include "JavaScriptOptional.h"
#include "JavaScriptPoint3D.h"
#include "JavaScriptColor.h"
#include "JavaScriptScalers2D.h"

#include "JavaScriptObject.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<UIText>::ToV8(v8::Isolate& isolate, const UIText& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        definition.string.SetValue(isolate, value.string);
        definition.font.SetValue(isolate, value.font);
        definition.material.SetValue(isolate, value.material);
        definition.wrapWidth.SetValue(isolate, value.wrapWidth);
        definition.bold.SetValue(isolate, value.bold);
        definition.italics.SetValue(isolate, value.italics);
        definition.color.SetValue(isolate, value.color);
        definition.position.SetValue(isolate, value.position);
        definition.scalers.SetValue(isolate, value.scalers);
        definition.rotation.SetValue(isolate, value.rotation);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<UIText> Type<UIText>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<UIText, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto id = definition.id.AtmosValue();
                const auto string = definition.string.AtmosValue();
                const auto font = definition.font.AtmosValue();
                const auto material = definition.material.AtmosValue();
                const auto wrapWidth = definition.wrapWidth.AtmosValue();
                const auto bold = definition.bold.AtmosValue();
                const auto italics = definition.italics.AtmosValue();
                const auto color = definition.color.AtmosValue();
                const auto position = definition.position.AtmosValue();
                const auto scalers = definition.scalers.AtmosValue();
                const auto rotation = definition.rotation.AtmosValue();
                return id && string && font && material && wrapWidth && bold && italics &&color && position && scalers && rotation
                    ? UIText
                    {
                        *id, *string, *font, *material, *wrapWidth, *bold, *italics, *color, *position, *scalers, *rotation
                    }
                    : std::optional<UIText>{};
            });
    }

    Type<UIText>::Definition::Definition() :
        id("id"),
        string("string"),
        font("font"),
        material("material"),
        wrapWidth("wrapWidth"),
        bold("bold"),
        italics("italics"),
        color("color"),
        position("position"),
        scalers("scalers"),
        rotation("rotation")
    {}

    AnyObjectDefinition Type<UIText>::Definition::ToAny()
    {
        return
        {
            {
                { id },
                { string },
                { font },
                { material },
                { wrapWidth },
                { bold },
                { italics },
                { color },
                { position },
                { scalers },
                { rotation }
            }
        };
    }

    auto Type<Arca::Index<UI::Text>>::ToV8(v8::Isolate& isolate, const Arca::Index<UI::Text>& value) -> v8::Local<V8T>
    {
        if (!value)
            return v8::Null(&isolate);

        const auto font = value->Font();
        const auto material = value->Material();
        return JavaScript::ToV8(
            isolate,
            UIText
            {
                value.ID(),
                value->String(),
                font
                    ? FontAsset
                    {
                        font.ID(),
                        font->Name()
                    }
                    : std::optional<FontAsset>(),
                material
                    ? MaterialAsset
                    {
                        material.ID(),
                        material->Name()
                    }
                    : std::optional<MaterialAsset>(),
                value->WrapWidth(),
                value->Bold(),
                value->Italics(),
                value->Color(),
                value->Position(),
                value->Scalers(),
                value->Rotation()
            });
    }

    std::optional<CreateUIText> Type<CreateUIText>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<CreateUIText, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto string = definition.string.AtmosValue();
                const auto font = definition.font.AtmosValue();
                const auto material = definition.material.AtmosValue();
                const auto wrapWidth = definition.wrapWidth.AtmosValue();
                const auto bold = definition.bold.AtmosValue();
                const auto italics = definition.italics.AtmosValue();
                const auto color = definition.color.AtmosValue();
                const auto position = definition.position.AtmosValue();
                const auto scalers = definition.scalers.AtmosValue();
                const auto rotation = definition.rotation.AtmosValue();
                return string && font && material && wrapWidth && bold && italics && color && position && scalers && rotation
                    ? CreateUIText
                    {
                        *string, *font, *material, *wrapWidth, *bold, *italics, *color, *position, *scalers, *rotation
                    }
                    : std::optional<CreateUIText>{};
            });
    }

    Type<CreateUIText>::Definition::Definition() :
        font("font"),
        string("string"),
        material("material"),
        wrapWidth("wrapWidth"),
        bold("bold"),
        italics("italics"),
        color("color"),
        position("position"),
        scalers("scalers"),
        rotation("rotation")
    {}

    AnyObjectDefinition Type<CreateUIText>::Definition::ToAny()
    {
        return
        {
            {
                { font },
                { string },
                { material },
                { wrapWidth },
                { bold },
                { italics },
                { color },
                { position },
                { scalers },
                { rotation }
            }
        };
    }

    std::optional<Arca::Create<UI::Text>> Type<Arca::Create<UI::Text>>::FromV8(
        v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        const auto userData = static_cast<UserData*>(isolate.GetData(0));
        const auto reliquary = userData->reliquary;

        const auto command = JavaScript::FromV8<CreateUIText>(isolate, value);
        return command
            ? Arca::Create<UI::Text>(
                command->string,
                command->font ? reliquary->Find<Asset::Font>(command->font->id) : Arca::Index<Asset::Font>(),
                command->material ? reliquary->Find<Asset::Material>(command->material->id) : Arca::Index<Asset::Material>(),
                command->wrapWidth,
                command->bold,
                command->italics,
                command->color,
                command->position,
                command->scalers,
                command->rotation)
            : std::optional<Arca::Create<UI::Text>>{};
    }
}