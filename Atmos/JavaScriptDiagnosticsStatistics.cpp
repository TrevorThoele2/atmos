#include "JavaScriptDiagnosticsStatistics.h"

#include "JavaScriptNumeric.h"
#include "JavaScriptDiagnosticsProfile.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<DiagnosticsStatistics>::ToV8(v8::Isolate& isolate, const DiagnosticsStatistics& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        definition.relicCount.SetValue(isolate, value.relicCount);
        definition.shardCount.SetValue(isolate, value.shardCount);
        definition.nextRelicID.SetValue(isolate, value.nextRelicID);
        definition.script.SetValue(isolate, value.script);
        definition.render.SetValue(isolate, value.render);
        definition.frame.SetValue(isolate, value.frame);
        definition.idle.SetValue(isolate, value.idle);
        definition.misc.SetValue(isolate, value.misc);
        definition.framesPerSecond.SetValue(isolate, value.framesPerSecond);
        return CreateObject(isolate, definition.ToAny());
    }

    Type<DiagnosticsStatistics>::Definition::Definition() :
        id("id"),
        relicCount("relicCount"),
        shardCount("shardCount"),
        nextRelicID("nextRelicId"),
        script("script"),
        render("render"),
        frame("frame"),
        idle("idle"),
        misc("misc"),
        framesPerSecond("framesPerSecond")
    {}

    AnyObjectDefinition Type<DiagnosticsStatistics>::Definition::ToAny()
    {
        return
        {
            {
                { id },
                { relicCount },
                { shardCount },
                { nextRelicID },
                { script },
                { render },
                { frame },
                { idle },
                { misc },
                { framesPerSecond }
            }
        };
    }

    auto Type<Arca::Index<Diagnostics::Statistics>>::ToV8(v8::Isolate& isolate, const Arca::Index<Diagnostics::Statistics>& value) -> v8::Local<V8T>
    {
        if (!value)
            return v8::Null(&isolate);

        return JavaScript::ToV8(
            isolate,
            DiagnosticsStatistics
            {
                value.ID(),
                value->relicCount,
                value->shardCount,
                value->nextRelicID,
                value->script,
                value->render,
                value->frame,
                value->idle,
                value->misc,
                value->framesPerSecond
            });
    }
}