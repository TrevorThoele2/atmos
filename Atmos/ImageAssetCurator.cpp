#include "ImageAssetCurator.h"

namespace Atmos::Asset
{
	ImageCurator::ImageCurator(Init init, ImageManager& manager) : Curator(init), manager(&manager)
	{}

	Resource::Loaded<Resource::Image> ImageCurator::Handle(const Resource::LoadDataFromFile<Resource::Image>& command)
    {
		return manager->Load(command.filePath);
    }

	Resource::Loaded<Resource::Image> ImageCurator::Handle(const Resource::LoadDataFromMemory<Resource::Image>& command)
	{
		return manager->Load(command.memory);
	}
}