#include "FontAssetCurator.h"

#include "InputSimpleFile.h"

#include <Chroma/Overloaded.h>

namespace Atmos::Asset
{
    Resource::LoadedData<Resource::Font> FontCurator::Handle(
        const Resource::LoadData<Resource::Font>& command)
    {
        return std::visit(Chroma::Overloaded{
            [](File::Path filePath) -> Resource::LoadedData<Resource::Font>
            {
                InputSimpleFile file(filePath);
                const auto returnBuffer = file.ReadBuffer();
                return { returnBuffer };
            },
            [](Buffer memory) -> Resource::LoadedData<Resource::Font>
            {
                return { memory };
            } },
            command.from);
    }
}