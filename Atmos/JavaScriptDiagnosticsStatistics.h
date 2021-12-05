#pragma once

#include "JavaScriptType.h"
#include "DiagnosticsStatistics.h"

#include "JavaScriptObject.h"
#include "JavaScriptRelicID.h"
#include <Arca/RelicID.h>

namespace Atmos::Scripting::JavaScript
{
    struct DiagnosticsStatistics
    {
        Arca::RelicID id;
        size_t relicCount;
        size_t shardCount;
        Arca::RelicID nextRelicID;
        Diagnostics::Statistics::Profile script;
        Diagnostics::Statistics::Profile render;
        Diagnostics::Statistics::Profile frame;
        Diagnostics::Statistics::Profile idle;
        Diagnostics::Statistics::Profile misc;
        Frame::FramesPerSecond framesPerSecond;
    };

    template<>
    class Type<DiagnosticsStatistics>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const DiagnosticsStatistics& value);
    private:
        struct Definition
        {
            Property<Arca::RelicID> id;
            Property<size_t> relicCount;
            Property<size_t> shardCount;
            Property<Arca::RelicID> nextRelicID;
            Property<Diagnostics::Statistics::Profile> script;
            Property<Diagnostics::Statistics::Profile> render;
            Property<Diagnostics::Statistics::Profile> frame;
            Property<Diagnostics::Statistics::Profile> idle;
            Property<Diagnostics::Statistics::Profile> misc;
            Property<Frame::FramesPerSecond> framesPerSecond;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };

    template<>
    class Type<Arca::Index<Diagnostics::Statistics>>
    {
    public:
        using V8T = v8::Value;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Arca::Index<Diagnostics::Statistics>& value);
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Scripting::JavaScript::DiagnosticsStatistics> final
    {
    public:
        using ObjectT = Atmos::Scripting::JavaScript::DiagnosticsStatistics;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            ScrivenRelicID("id", object.id, format);
            format("relicCount", object.relicCount);
            format("shardCount", object.shardCount);
            ScrivenRelicID("nextRelicId", object.nextRelicID, format);
            format("script", object.script);
            format("render", object.render);
            format("frame", object.frame);
            format("idle", object.idle);
            format("misc", object.misc);
            format("framesPerSecond", object.framesPerSecond);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Scripting::JavaScript::DiagnosticsStatistics, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Scripting::JavaScript::DiagnosticsStatistics>;
    };
}