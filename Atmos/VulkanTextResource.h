#pragma once

#include "TextResource.h"
#include "VulkanImageResourceData.h"

namespace Atmos::Render::Resource::Vulkan
{
    class Text final : public Resource::Text
    {
    public:
        ImageData imageData;
    public:
        Text(const ImageData& imageData);
    };
}