#include "ImageAssetCurator.h"

#include <Chroma/Overloaded.h>

namespace Atmos::Asset
{
	ImageCurator::ImageCurator(Init init, Resource::Manager& resourceManager) :
        Curator(init), resourceManager(&resourceManager)
	{}

	Resource::LoadedData<Resource::Image> ImageCurator::Handle(const Resource::LoadData<Resource::Image>& command)
    {
        return std::visit(Chroma::Overloaded{
            [this](File::Path filePath) -> Resource::LoadedData<Resource::Image>
            {
                return resourceManager->LoadImageData(filePath);
            },
            [this](Buffer memory) -> Resource::LoadedData<Resource::Image>
            {
                return resourceManager->LoadImageData(memory);
            } },
            command.from);

    }
}