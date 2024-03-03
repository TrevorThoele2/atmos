#include "ScriptAssetCurator.h"

namespace Atmos::Asset
{
    Resource::Loaded<Resource::Script> ScriptCurator::Handle(
        const Resource::LoadDataFromFile<Resource::Script>& command)
    {
        auto inputArchive = Inscription::InputBinaryArchive(command.filePath);
        const auto size = inputArchive.Size();

        Inscription::Buffer loadedBuffer;
        loadedBuffer.value.resize(static_cast<size_t>(size));
        inputArchive.Read(loadedBuffer);

        const auto returnBuffer = Buffer(loadedBuffer.value.begin(), loadedBuffer.value.end());

        return { returnBuffer };
    }

    Resource::Loaded<Resource::Script> ScriptCurator::Handle(
        const Resource::LoadDataFromMemory<Resource::Script>& command)
    {
        return { command.memory };
    }
}