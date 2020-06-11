#include "VulkanImageAssetData.h"

namespace Atmos::Render::Vulkan
{
    ImageAssetDataImplementation::ImageAssetDataImplementation(
        Image&& image,
        vk::UniqueImageView&& imageView,
        CombinedImageSamplerDescriptor descriptor)
        :
        image(std::move(image)),
        imageView(std::move(imageView)),
        descriptor(std::move(descriptor))
    {}
}