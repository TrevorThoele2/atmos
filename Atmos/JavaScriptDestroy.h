#pragma once

#include "JavaScriptType.h"
#include "JavaScriptObject.h"
#include "JavaScriptRelicID.h"
#include <Arca/Destroy.h>

namespace Atmos::Scripting::JavaScript
{
    template<class T>
    struct Destroy
    {
        Arca::RelicID id;
    };

    template<class T>
    class Type<Destroy<T>>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static std::optional<Destroy<T>> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Arca::RelicID> id;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };

    template<class T>
	std::optional<Destroy<T>> Type<Destroy<T>>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Destroy<T>, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto id = definition.id.AtmosValue();
                return id
                    ? Destroy<T>
	                {
	                    *id
	                }
	                : std::optional<Destroy<T>>{};
            });
    }

    template<class T>
    Type<Destroy<T>>::Definition::Definition() :
		id("id")
    {}

    template<class T>
	AnyObjectDefinition Type<Destroy<T>>::Definition::ToAny()
    {
        return
        {
            { id }
        };
    }

    template<class T>
    class Type<Arca::Destroy<T>>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static std::optional<Arca::Destroy<T>> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };

    template<class T>
    std::optional<Arca::Destroy<T>> Type<Arca::Destroy<T>>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        const auto destroy = JavaScript::FromV8<Destroy<T>>(isolate, value);
        return destroy ? Arca::Destroy<T>{destroy->id} : std::optional<Arca::Destroy<T>>{};
    }
}

namespace Inscription
{
    template<class T>
    class Scribe<Atmos::Scripting::JavaScript::Destroy<T>> final
    {
    public:
        using ObjectT = Atmos::Scripting::JavaScript::Destroy<T>;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            ScrivenRelicID("id", object.id, format);
        }
    };

    template<class T, class Format>
    struct ScribeTraits<Atmos::Scripting::JavaScript::Destroy<T>, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::Destroy<T>>;
    };
}