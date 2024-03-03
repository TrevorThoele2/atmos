#pragma once

#include <vector>
#include <map>
#include "VulkanIncludes.h"
#include "MaterialAsset.h"
#include "Point3D.h"

namespace Atmos::Render::Vulkan
{
    template<class Context, class Addition>
    struct DrawContext
    {
        Addition addition;
        std::vector<const Asset::Material*> materials;
        vk::CommandBuffer commandBuffer;
        uint32_t layerCount = 0;
        typename std::map<Spatial::Point3D::Value, Context>::iterator currentLayer;
    };
}