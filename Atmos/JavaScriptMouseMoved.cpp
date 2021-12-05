#include "JavaScriptMouseMoved.h"

#include "JavaScriptPoint2D.h"
#include "JavaScriptNumeric.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Input::MouseMoved>::ToV8(v8::Isolate& isolate, const Input::MouseMoved& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.previous.SetValue(isolate, value.previous);
        definition.current.SetValue(isolate, value.current);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Input::MouseMoved> Type<Input::MouseMoved>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Input::MouseMoved, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto previous = definition.previous.AtmosValue();
                const auto current = definition.current.AtmosValue();
                return previous && current
                    ? Input::MouseMoved
	                {
	                    *previous, *current
	                }
	                : std::optional<Input::MouseMoved>{};
            });
    }

    Type<Input::MouseMoved>::Definition::Definition() :
        previous("previous"),
        current("current")
    {}

    AnyObjectDefinition Type<Input::MouseMoved>::Definition::ToAny()
    {
        return
        {
            {
                { previous },
                { current }
            }
        };
    }
}