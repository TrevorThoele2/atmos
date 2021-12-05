#pragma once

#include "JavaScriptType.h"
#include "ViewSlice.h"

#include "JavaScriptImageAsset.h"

#include "JavaScriptObject.h"
#include "JavaScriptRelicID.h"
#include <Arca/ShardIndex.h>
#include <Arca/RelicID.h>
#include <Arca/Create.h>

namespace Atmos::Scripting::JavaScript
{
    struct ViewSlice
    {
        Arca::RelicID id;
        Spatial::AxisAlignedBox2D box;
    };

    template<>
    class Type<ViewSlice>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const ViewSlice& value);
    private:
        struct Definition
        {
            Property<Arca::RelicID> id;
            Property<Spatial::AxisAlignedBox2D> box;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };

    template<>
    class Type<Arca::Index<Render::ViewSlice>>
    {
    public:
        using V8T = v8::Value;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Arca::Index<Render::ViewSlice>& value);
    };

    struct CreateViewSlice
    {
        Arca::RelicID id = 0;
        Spatial::AxisAlignedBox2D box;
    };

    template<>
    class Type<CreateViewSlice>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static std::optional<CreateViewSlice> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Arca::RelicID> id;
            Property<Spatial::AxisAlignedBox2D> box;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };

    template<>
    class Type<Arca::Create<Render::ViewSlice>>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static std::optional<Arca::Create<Render::ViewSlice>> FromV8(
            v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Scripting::JavaScript::ViewSlice> final
    {
    public:
        using ObjectT = Atmos::Scripting::JavaScript::ViewSlice;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            ScrivenRelicID("id", object.id, format);
            format("box", object.box);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Scripting::JavaScript::ViewSlice, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::ViewSlice>;
    };

    template<>
    class Scribe<Atmos::Scripting::JavaScript::CreateViewSlice> final
    {
    public:
        using ObjectT = Atmos::Scripting::JavaScript::CreateViewSlice;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            ScrivenRelicID("id", object.id, format);
            format("box", object.box);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Scripting::JavaScript::CreateViewSlice, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::CreateViewSlice>;
    };
}