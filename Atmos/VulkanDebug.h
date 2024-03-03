#pragma once

#include "VulkanIncludes.h"

#include "Logger.h"

namespace Atmos::Render::Vulkan
{
    class Debug
    {
    public:
        static Logging::Logger* logger;
    public:
        explicit Debug(vk::Instance instance);
        ~Debug();

        [[nodiscard]] VkDebugUtilsMessengerCreateInfoEXT CreateInfo();
    private:
        VkDebugUtilsMessengerEXT messenger = {};

        static VKAPI_ATTR VkBool32 VKAPI_CALL Callback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);
    private:
        vk::Instance instance;
    };
}