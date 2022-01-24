#include "JavaScriptActionPressed.h"

#include "JavaScriptOptional.h"

#include "JavaScriptObject.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Input::ActionPressed>::ToV8(v8::Isolate& isolate, const Input::ActionPressed& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.action.SetValue(
            isolate,
            ActionAsset
            {
                value.action.ID(),
                value.action->Name(),
                value.action->boundKey,
                std::vector<Input::Key>{value.action->boundModifiers.begin(), value.action->boundModifiers.end()}
            });
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Input::ActionPressed> Type<Input::ActionPressed>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Input::ActionPressed, Definition>(
            isolate,
            value,
            [&isolate](const Definition& definition)
            {
                const auto userData = static_cast<UserData*>(isolate.GetData(0));
                const auto reliquary = userData->reliquary;

                const auto action = definition.action.AtmosValue();
                return action
                    ? Input::ActionPressed
                    {
                        *action ? reliquary->Find<Asset::Action>((*action)->id) : Arca::Index<Asset::Action>()
                    }
                    : std::optional<Input::ActionPressed>{};
            });
    }

    Type<Input::ActionPressed>::Definition::Definition() :
        action("action")
    {}

    AnyObjectDefinition Type<Input::ActionPressed>::Definition::ToAny()
    {
        return
        {
            {
                { action }
            }
        };
    }
}