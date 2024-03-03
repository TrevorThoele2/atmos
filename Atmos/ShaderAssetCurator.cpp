#include "ShaderAssetCurator.h"

#include "InputSimpleFile.h"

namespace Atmos::Asset
{
    Resource::Loaded<Resource::Shader> ShaderCurator::Handle(
        const Resource::LoadDataFromFile<Resource::Shader>& command)
    {
        InputSimpleFile file(command.filePath);
        const auto returnBuffer = file.ReadBuffer();
        return { returnBuffer };
    }

    Resource::Loaded<Resource::Shader> ShaderCurator::Handle(
        const Resource::LoadDataFromMemory<Resource::Shader>& command)
    {
        return { command.memory };
    }
}