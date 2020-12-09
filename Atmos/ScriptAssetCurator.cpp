#include "ScriptAssetCurator.h"

namespace Atmos::Asset
{
    Resource::Loaded<Resource::Script> ScriptCurator::Handle(
        const Resource::LoadDataFromFile<Resource::Script>& command)
    {
        auto inputArchive = Inscription::InputBinaryArchive(command.filePath);
        const auto size = inputArchive.Size();

        Inscription::Buffer loadedBuffer;
        loadedBuffer.resize(static_cast<size_t>(size));
        inputArchive.Read(loadedBuffer);

        const auto returnBuffer = DataBuffer(loadedBuffer.begin(), loadedBuffer.end());

        return { returnBuffer };
    }

    Resource::Loaded<Resource::Script> ScriptCurator::Handle(
        const Resource::LoadDataFromMemory<Resource::Script>& command)
    {
        return { command.memory };
    }
}