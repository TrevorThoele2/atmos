
#include "ConsumableAspect.h"
#include "ScriptRegistry.h"
#include <Inscription/Scribe.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(ConsumableAspect)
    {
        scribe(script);
    }

    ConsumableAspect::ConsumableAspect(ConsumableAspect &&arg) : script(std::move(arg.script))
    {}

    ConsumableAspect& ConsumableAspect::operator=(ConsumableAspect &&arg)
    {
        script = std::move(arg.script);
        return *this;
    }

    bool ConsumableAspect::operator==(const ConsumableAspect &arg) const
    {
        return script == arg.script;
    }

    bool ConsumableAspect::operator!=(const ConsumableAspect &arg) const
    {
        return !(*this == arg);
    }

    void ConsumableAspect::Affect(Entity affect) const
    {
        Script::Instance instance(static_cast<const Script*>(script.Get())->MakeInstance());
        instance.SetOrAddGlobalItem("entity", *affect);
        instance.Execute();
    }
}