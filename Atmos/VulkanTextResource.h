#pragma once

#include "TextResource.h"
#include "VulkanImage.h"

namespace Atmos::Render::Resource::Vulkan
{
    class Text final : public Resource::Text
    {
    public:
        Render::Vulkan::ImageData imageData;
    public:
        explicit Text(const Render::Vulkan::ImageData& imageData);
    };
}