#include "AngelScriptPaths.h"

namespace Atmos::Scripting::Angel
{
    File::Path ToCompiledPath(const File::Path& filePath)
    {
        return File::Path(filePath).replace_extension("asc");
    }
}