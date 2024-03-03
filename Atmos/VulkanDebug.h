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

        [[nodiscard]] static VkDebugUtilsMessengerCreateInfoEXT CreateInfo();
    private:
        VkDebugUtilsMessengerEXT messenger = {};

        static VKAPI_ATTR VkBool32 VKAPI_CALL Callback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);
    private:
        [[nodiscard]] static bool ShouldSkip(const String& string);
    private:
        struct ObjectInfo
        {
            String type;
            String name;
        };

        enum class MessageType
        {
            General,
            Validation,
            Performance
        };

        [[nodiscard]] static std::vector<String> ObjectStrings(const VkDebugUtilsMessengerCallbackDataEXT& data);
        [[nodiscard]] static std::vector<String> TypeStrings(VkDebugUtilsMessageTypeFlagsEXT type);

        [[nodiscard]] static ObjectInfo From(const VkDebugUtilsObjectNameInfoEXT& info);
        [[nodiscard]] static Logging::Severity From(VkDebugUtilsMessageSeverityFlagBitsEXT severity);
        [[nodiscard]] static std::vector<MessageType> From(VkDebugUtilsMessageTypeFlagsEXT type);
        [[nodiscard]] static String From(MessageType type);
    private:
        vk::Instance instance;
    };
}