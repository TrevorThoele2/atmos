#include "ShaderAssetCurator.h"

#include "InputSimpleFile.h"

#include <Chroma/Overloaded.h>

namespace Atmos::Asset
{
    Resource::LoadedData<Resource::Shader> ShaderCurator::Handle(
        const Resource::LoadData<Resource::Shader>& command)
    {
        return std::visit(Chroma::Overloaded{
            [](File::Path filePath) -> Resource::LoadedData<Resource::Shader>
            {
                InputSimpleFile file(filePath);
                const auto returnBuffer = file.ReadBuffer();
                return { returnBuffer };
            },
            [](Buffer memory) -> Resource::LoadedData<Resource::Shader>
            {
                return { memory };
            } },
            command.from);
    }
}