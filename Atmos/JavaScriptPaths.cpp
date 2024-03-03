#include "JavaScriptPaths.h"

namespace Atmos::Scripting::JavaScript
{
    File::Path ToCompiledPath(const File::Path& filePath)
    {
        return File::Path(filePath).replace_extension("js");
    }
}