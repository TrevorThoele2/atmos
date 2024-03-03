#include "VulkanImageAssetData.h"

namespace Atmos::Render::Vulkan
{
    ImageAssetDataImplementation::ImageAssetDataImplementation(
        vk::UniqueImage image,
        vk::UniqueDeviceMemory memory,
        vk::UniqueImageView imageView,
        CombinedImageSamplerDescriptor descriptor,
        Dimension width,
        Dimension height)
        :
        ImageAssetData(width, height),
        image(std::move(image)),
        memory(std::move(memory)),
        imageView(std::move(imageView)),
        descriptor(descriptor)
    {}
}