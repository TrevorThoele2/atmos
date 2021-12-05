#include "VulkanDebug.h"

#include "VulkanResults.h"

#include "GraphicsError.h"
#include "Logger.h"

namespace Atmos::Render::Vulkan
{
    Logging::Logger* Debug::logger = nullptr;

    Debug::Debug(vk::Instance instance) : instance(instance)
    {
        const auto createInfo = CreateInfo();
        const auto createDebugUtilsMessenger = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
            instance.getProcAddr("vkCreateDebugUtilsMessengerEXT"));
        if (!createDebugUtilsMessenger)
            throw GraphicsError("Could not retrieve debug messenger creation method.");

        if (IsError(createDebugUtilsMessenger(static_cast<VkInstance>(instance), &createInfo, nullptr, &messenger)))
            throw GraphicsError("Could not create debug messenger.");
    }

    Debug::~Debug()
    {
        const auto destroyDebugUtilsMessenger = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
            instance.getProcAddr("vkDestroyDebugUtilsMessengerEXT"));
        if (destroyDebugUtilsMessenger != nullptr)
            destroyDebugUtilsMessenger(static_cast<VkInstance>(instance), messenger, nullptr);
    }

    VkDebugUtilsMessengerCreateInfoEXT Debug::CreateInfo()
    {
        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = &Callback;
        createInfo.pUserData = nullptr;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        return createInfo;
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL Debug::Callback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData)
    {
        if (messageSeverity < VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
            return VK_FALSE;

        const auto message = pCallbackData->pMessage ? pCallbackData->pMessage : String("No message.");
        if (ShouldSkip(message))
            return VK_FALSE;

        logger->Log(String(message), From(messageSeverity));

        return VK_FALSE;
    }

    bool Debug::ShouldSkip(const String& string)
    {
        return Chroma::Contains(string, "UNASSIGNED-CoreValidation-DrawState-InvalidImageLayout");
    }

    Logging::Severity Debug::From(VkDebugUtilsMessageSeverityFlagBitsEXT severity)
    {
        switch (severity)
        {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            return Logging::Severity::Verbose;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            return Logging::Severity::Information;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            return Logging::Severity::Warning;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            return Logging::Severity::Error;
        default:
            return Logging::Severity::Error;
        }
    }
}