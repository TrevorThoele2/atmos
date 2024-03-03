#include "JavaScriptInputInformation.h"

#include "JavaScriptPoint2D.h"
#include "JavaScriptNumeric.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<InputInformation>::ToV8(v8::Isolate& isolate, const InputInformation& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        definition.previousMousePosition.SetValue(isolate, value.previousMousePosition);
        definition.currentMousePosition.SetValue(isolate, value.currentMousePosition);
        return CreateObject(isolate, definition.ToAny());
    }

    Type<InputInformation>::Definition::Definition() :
        id("id"),
        previousMousePosition("previousMousePosition"),
        currentMousePosition("currentMousePosition")
    {}

    AnyObjectDefinition Type<InputInformation>::Definition::ToAny()
    {
        return
        {
            {
                { id },
                { previousMousePosition },
                { currentMousePosition }
            }
        };
    }

    auto Type<Arca::Index<Input::Information>>::ToV8(v8::Isolate& isolate, const Arca::Index<Input::Information>& value) -> v8::Local<V8T>
    {
        if (!value)
            return v8::Null(&isolate);

        return JavaScript::ToV8(
            isolate,
            InputInformation
            {
                value.ID(),
                value->mousePosition.previous,
                value->mousePosition.current
            });
    }
}