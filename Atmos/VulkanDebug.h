#pragma once

#include "VulkanIncludes.h"

namespace Atmos::Render::Vulkan
{
    class Debug
    {
    public:
        Debug() = default;
        ~Debug();

        void Initialize(vk::Instance instance);

        [[nodiscard]] VkDebugUtilsMessengerCreateInfoEXT CreateInfo();
    private:
        bool initialized = false;
    private:
        VkDebugUtilsMessengerEXT messenger;

        static VKAPI_ATTR VkBool32 VKAPI_CALL Callback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);
    private:
        vk::Instance instance;
    };
}