#include "JavaScriptActionAsset.h"

#include "JavaScriptKey.h"
#include "JavaScriptVector.h"
#include "JavaScriptNumeric.h"

#include "JavaScriptObject.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<ActionAsset>::ToV8(v8::Isolate& isolate, const ActionAsset& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        definition.name.SetValue(isolate, value.name);
        definition.boundKey.SetValue(isolate, value.boundKey);
        definition.boundModifiers.SetValue(isolate, value.boundModifiers);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<ActionAsset> Type<ActionAsset>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<ActionAsset, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto id = definition.id.AtmosValue();
                const auto name = definition.name.AtmosValue();
                const auto boundKey = definition.boundKey.AtmosValue();
                const auto boundModifiers = definition.boundModifiers.AtmosValue();
                return id && name && boundKey && boundModifiers
                    ? ActionAsset
                    {
                        *id, *name, *boundKey, *boundModifiers
                    }
                    : std::optional<ActionAsset>{};
            });
    }
    
    Type<ActionAsset>::Definition::Definition() :
		id("id"),
        name("name"),
        boundKey("boundKey"),
        boundModifiers("boundModifiers")
    {}

    AnyObjectDefinition Type<ActionAsset>::Definition::ToAny()
    {
        return
        {
            {
                { id },
                { name },
                { boundKey },
                { boundModifiers }
            }
        };
    }

    auto Type<Arca::Index<Asset::Action>>::ToV8(v8::Isolate& isolate, const Arca::Index<Asset::Action>& value) -> v8::Local<V8T>
    {
        return JavaScript::ToV8(
            isolate,
            ActionAsset
            {
                value.ID(),
                value->Name(),
                value->boundKey,
                std::vector<Input::Key>{value->boundModifiers.begin(), value->boundModifiers.end()}
            });
    }
}