#include "ExecuteImmediately.h"

namespace Atmos::Script
{
    ExecuteImmediately::ExecuteImmediately(RunningScript& script) : script(script)
    {}
}

namespace Arca
{
    const TypeName Traits<Atmos::Script::ExecuteImmediately>::typeName = "ScriptExecuteImmediately";
}