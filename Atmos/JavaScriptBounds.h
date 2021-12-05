#pragma once

#include "JavaScriptType.h"
#include "Bounds.h"

#include "JavaScriptObject.h"
#include "JavaScriptRelicID.h"
#include <Arca/ShardIndex.h>
#include <Arca/RelicID.h>
#include <Arca/Create.h>

namespace Atmos::Scripting::JavaScript
{
    struct Bounds
    {
        Arca::RelicID id;
        Spatial::Space space;
        Spatial::Point3D position;
        Spatial::Size2D baseSize;
        Spatial::Scalers2D scalers;
        Spatial::Angle2D rotation;
    };

    template<>
    class Type<Bounds>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Bounds& value);
    private:
        struct Definition
        {
            Property<Arca::RelicID> id;
            Property<Spatial::Space> space;
            Property<Spatial::Point3D> position;
            Property<Spatial::Size2D> baseSize;
            Property<Spatial::Scalers2D> scalers;
            Property<Spatial::Angle2D> rotation;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };

    template<>
    class Type<Arca::Index<Spatial::Bounds>>
    {
    public:
        using V8T = v8::Value;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Arca::Index<Spatial::Bounds>& value);
    };

    struct CreateBounds
    {
        Arca::RelicID id = 0;
        Spatial::Space space = Spatial::Space::World;
        Spatial::Point3D position;
        Spatial::Size2D baseSize;
        Spatial::Scalers2D scalers;
        Spatial::Angle2D rotation = 0.0f;
    };
    
    template<>
    class Type<CreateBounds>
    {
    public:
        using V8T = v8::Object;
        
        [[nodiscard]] static std::optional<CreateBounds> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Arca::RelicID> id;
            Property<Spatial::Space> space;
            Property<Spatial::Point3D> position;
            Property<Spatial::Size2D> baseSize;
            Property<Spatial::Scalers2D> scalers;
            Property<Spatial::Angle2D> rotation;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };

    template<>
    class Type<Arca::Create<Spatial::Bounds>>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static std::optional<Arca::Create<Spatial::Bounds>> FromV8(
            v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Scripting::JavaScript::Bounds> final
    {
    public:
        using ObjectT = Atmos::Scripting::JavaScript::Bounds;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            ScrivenRelicID("id", object.id, format);
            format("space", object.space);
            format("position", object.position);
            format("baseSize", object.baseSize);
            format("scalers", object.scalers);
            format("rotation", object.rotation);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Scripting::JavaScript::Bounds, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::Bounds>;
    };

    template<>
    class Scribe<Atmos::Scripting::JavaScript::CreateBounds> final
    {
    public:
        using ObjectT = Atmos::Scripting::JavaScript::CreateBounds;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            ScrivenRelicID("id", object.id, format);
            format("space", object.space);
            format("position", object.position);
            format("baseSize", object.baseSize);
            format("scalers", object.scalers);
            format("rotation", object.rotation);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Scripting::JavaScript::CreateBounds, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::CreateBounds>;
    };
}