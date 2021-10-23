#include "VulkanPaths.h"

#include <Chroma/StringUtility.h>

namespace Atmos::Render::Vulkan
{
    File::Path ToCompiledPath(const File::Path& filePath)
    {
        return File::Path(filePath).replace_extension("spv");
    }
}
