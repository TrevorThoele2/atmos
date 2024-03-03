#pragma once

#include "JavaScriptType.h"
#include "Camera.h"

#include "JavaScriptObject.h"
#include "JavaScriptRelicID.h"
#include <Arca/RelicID.h>

namespace Atmos::Scripting::JavaScript
{
    struct Camera
    {
        Arca::RelicID id;
        Spatial::Point3D position;
        Spatial::Size2D size;
        Spatial::AxisAlignedBox2D sides;
    };

    template<>
    class Type<Camera>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Camera& value);
    private:
        struct Definition
        {
            Property<Arca::RelicID> id;
            Property<Spatial::Point3D> position;
            Property<Spatial::Size2D> size;
            Property<Spatial::AxisAlignedBox2D> sides;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };

    template<>
    class Type<Arca::Index<Render::Camera>>
    {
    public:
        using V8T = v8::Value;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Arca::Index<Render::Camera>& value);
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Scripting::JavaScript::Camera> final
    {
    public:
        using ObjectT = Atmos::Scripting::JavaScript::Camera;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            ScrivenRelicID("id", object.id, format);
            format("position", object.position);
            format("size", object.size);
            format("sides", object.sides);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Scripting::JavaScript::Camera, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::Camera>;
    };
}