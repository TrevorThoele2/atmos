#pragma once

#include "JavaScriptType.h"
#include "InputInformation.h"

#include "JavaScriptObject.h"
#include "JavaScriptRelicID.h"
#include <Arca/RelicID.h>

namespace Atmos::Scripting::JavaScript
{
    struct InputInformation
    {
        Arca::RelicID id;
        Spatial::Point2D previousMousePosition;
        Spatial::Point2D currentMousePosition;
    };

    template<>
    class Type<InputInformation>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const InputInformation& value);
    private:
        struct Definition
        {
            Property<Arca::RelicID> id;
            Property<Spatial::Point2D> previousMousePosition;
            Property<Spatial::Point2D> currentMousePosition;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };

    template<>
    class Type<Arca::Index<Input::Information>>
    {
    public:
        using V8T = v8::Value;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Arca::Index<Input::Information>& value);
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Scripting::JavaScript::InputInformation> final
    {
    public:
        using ObjectT = Atmos::Scripting::JavaScript::InputInformation;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            ScrivenRelicID("id", object.id, format);
            format("previousMousePosition", object.previousMousePosition);
            format("currentMousePosition", object.currentMousePosition);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Scripting::JavaScript::InputInformation, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::InputInformation>;
    };
}