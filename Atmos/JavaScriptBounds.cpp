#include "JavaScriptBounds.h"

#include "JavaScriptNumeric.h"
#include "JavaScriptSpace.h"
#include "JavaScriptPoint3D.h"
#include "JavaScriptSize2D.h"
#include "JavaScriptScalers2D.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Bounds>::ToV8(v8::Isolate& isolate, const Bounds& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        definition.space.SetValue(isolate, value.space);
        definition.position.SetValue(isolate, value.position);
        definition.baseSize.SetValue(isolate, value.baseSize);
        definition.scalers.SetValue(isolate, value.scalers);
        definition.rotation.SetValue(isolate, value.rotation);
        return CreateObject(isolate, definition.ToAny());
    }

    Type<Bounds>::Definition::Definition() :
        id("id"),
        space("space"),
        position("position"),
        baseSize("baseSize"),
		scalers("scalers"),
		rotation("rotation")
    {}

    AnyObjectDefinition Type<Bounds>::Definition::ToAny()
    {
        return
        {
            {
                { id },
                { space },
                { position },
                { baseSize },
                { scalers },
                { rotation }
            }
        };
    }

    auto Type<Arca::Index<Spatial::Bounds>>::ToV8(v8::Isolate& isolate, const Arca::Index<Spatial::Bounds>& value) -> v8::Local<V8T>
    {
        if (!value)
            return v8::Null(&isolate);

        return JavaScript::ToV8(
            isolate,
            Bounds
            {
                value.ID(),
                value->Space(),
                value->Position(),
                value->BaseSize(),
                value->Scalers(),
                value->Rotation()
            });
    }

    std::optional<CreateBounds> Type<CreateBounds>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<CreateBounds, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto id = definition.id.AtmosValue();
                const auto space = definition.space.AtmosValue();
                const auto position = definition.position.AtmosValue();
                const auto baseSize = definition.baseSize.AtmosValue();
                const auto scalers = definition.scalers.AtmosValue();
                const auto rotation = definition.rotation.AtmosValue();
                return id && space && position && baseSize && scalers && rotation
                    ? CreateBounds
	                {
	                    *id, *space, *position, *baseSize, *scalers, *rotation
	                }
	                : std::optional<CreateBounds>{};
            });
    }

    Type<CreateBounds>::Definition::Definition() :
		id("id"),
        space("space"),
        position("position"),
        baseSize("baseSize"),
        scalers("scalers"),
        rotation("rotation")
    {}

    AnyObjectDefinition Type<CreateBounds>::Definition::ToAny()
    {
        return
        {
            {
                { id },
                { space },
                { position },
                { baseSize },
                { scalers },
                { rotation }
            }
        };
    }

    std::optional<Arca::Create<Spatial::Bounds>> Type<Arca::Create<Spatial::Bounds>>::FromV8(
        v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        const auto command = JavaScript::FromV8<CreateBounds>(isolate, value);
        return command
            ? Arca::Create<Spatial::Bounds>(command->id, command->space, command->position, command->baseSize, command->scalers, command->rotation)
            : std::optional<Arca::Create<Spatial::Bounds>>{};
    }
}