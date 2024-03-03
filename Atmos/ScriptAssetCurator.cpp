#include "ScriptAssetCurator.h"

#include <Chroma/Overloaded.h>

namespace Atmos::Asset
{
    Resource::LoadedData<Resource::Script> ScriptCurator::Handle(
        const Resource::LoadData<Resource::Script>& command)
    {
        return std::visit(Chroma::Overloaded {
            [](File::Path filePath) -> Resource::LoadedData<Resource::Script>
            {
                auto inputArchive = Inscription::Archive::InputBinary(filePath);
                const auto size = inputArchive.Size();

                Inscription::Buffer loadedBuffer;
                loadedBuffer.value.resize(static_cast<size_t>(size));
                inputArchive.Read(loadedBuffer);

                const auto returnBuffer = Buffer(loadedBuffer.value.begin(), loadedBuffer.value.end());

                return { returnBuffer };
            },
            [](Buffer memory) -> Resource::LoadedData<Resource::Script>
            {
                return { memory };
            }},
            command.from);
    }
}