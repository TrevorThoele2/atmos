#pragma once

#include "VulkanIncludes.h"

#include "Color.h"

#include <glm/glm.hpp>

namespace Atmos::Render::Vulkan
{
    bool IsSuccess(vk::Result result);
    bool IsSuccess(VkResult result);
    bool IsError(vk::Result result);
    bool IsError(VkResult result);

    template<class NonUniqueT, class UniqueT>
    std::vector<NonUniqueT> UniqueToNonUnique(const std::vector<UniqueT>& unique)
    {
        std::vector<NonUniqueT> nonUnique;
        nonUnique.reserve(unique.size());
        for (auto& uniqueElement : unique)
            nonUnique.push_back(uniqueElement.get());
        return nonUnique;
    }

    glm::vec4 AtmosToVulkanColor(const Color& color);
}