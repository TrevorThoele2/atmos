#include "VulkanDebug.h"

#include "VulkanUtilities.h"

#include "GraphicsError.h"
#include "Logger.h"

namespace Atmos::Render::Vulkan
{
    Debug::~Debug()
    {
        if (!initialized)
            return;

        const auto destroyDebugUtilsMessenger = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
            instance.getProcAddr("vkDestroyDebugUtilsMessengerEXT"));
        if (destroyDebugUtilsMessenger != nullptr)
            destroyDebugUtilsMessenger(static_cast<VkInstance>(instance), messenger, nullptr);

        initialized = false;
    }

    void Debug::Initialize(vk::Instance instance)
    {
        if (initialized)
            return;

        const auto createInfo = CreateInfo();
        const auto createDebugUtilsMessenger = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
            instance.getProcAddr("vkCreateDebugUtilsMessengerEXT"));
        if (!createDebugUtilsMessenger)
            throw GraphicsError("Could not retrieve debug messenger creation method.");

        if (IsError(createDebugUtilsMessenger(static_cast<VkInstance>(instance), &createInfo, nullptr, &messenger)))
            throw GraphicsError("Could not create debug messenger.");

        this->instance = instance;
        initialized = true;
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
        Logging::Severity severity;
        if (messageSeverity < VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
            return VK_FALSE;

        switch (messageSeverity)
        {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            severity = Logging::Severity::Verbose;
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            severity = Logging::Severity::Information;
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            severity = Logging::Severity::Warning;
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            severity = Logging::Severity::Error;
            break;
        default:
            severity = Logging::Severity::Error;
            break;
        }

        const auto message = pCallbackData->pMessage ? pCallbackData->pMessage : String("No message.");

        Logging::logger.Log(
            String(message),
            severity);

        return VK_FALSE;
    }
}