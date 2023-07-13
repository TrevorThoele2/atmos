#pragma once

#include "JavaScriptType.h"
#include "ExecutingMaterial.h"

#include "JavaScriptRasterImage.h"
#include "JavaScriptRasterText.h"
#include "JavaScriptRasterLine.h"
#include "JavaScriptRasterRegion.h"
#include "JavaScriptRasterCommand.h"
#include "JavaScriptVariant.h"
#include "JavaScriptVector.h"
#include "JavaScriptObject.h"
#include "JavaScriptRelicID.h"
#include <Arca/RelicID.h>

namespace Atmos::Scripting::JavaScript
{
    struct ExecutingMaterial
    {
        Arca::RelicID id;
        std::vector<Render::Raster::Image> images;
        std::vector<Render::Raster::Text> texts;
        std::vector<Render::Raster::Line> lines;
        std::vector<Render::Raster::Region> regions;

        std::vector<Render::Raster::Command> recordedCommands = {};
    };

    template<>
    class Type<ExecutingMaterial>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const ExecutingMaterial& value);
    private:
        struct Definition
        {
            Property<Arca::RelicID> id;
            Property<std::vector<Render::Raster::Image>> images;
            Property<std::vector<Render::Raster::Text>> texts;
            Property<std::vector<Render::Raster::Line>> lines;
            Property<std::vector<Render::Raster::Region>> regions;

            Property<std::vector<Render::Raster::Command>> recordedCommands;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };

    template<>
    class Type<Arca::Index<Render::Raster::ExecutingMaterial>>
    {
    public:
        using V8T = v8::Value;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Arca::Index<Render::Raster::ExecutingMaterial>& value);
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Scripting::JavaScript::ExecutingMaterial> final
    {
    public:
        using ObjectT = Atmos::Scripting::JavaScript::ExecutingMaterial;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            ScrivenRelicID("id", object.id, format);
            format("images", object.images);
            format("texts", object.texts);
            format("lines", object.lines);
            format("regions", object.regions);
            format("recordedCommands", object.recordedCommands);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Scripting::JavaScript::ExecutingMaterial, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::ExecutingMaterial>;
    };
}