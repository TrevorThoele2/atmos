#include "AngelScriptAssetResource.h"

#include <angelscript.h>

namespace Atmos::Scripting::Angel::Resource
{
    Script::Script(asIScriptModule& module) : module(&module)
    {}

    asIScriptModule* Script::Module() const
    {
        return module;
    }
}