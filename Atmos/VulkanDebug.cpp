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

        const auto messageTypes = From(messageType);

        const auto messageIdNumber = pCallbackData->messageIdNumber;
        const auto messageIdName = String(pCallbackData->pMessageIdName);

        Logging::Details details;

        const auto typeStrings = TypeStrings(messageType);
        if (!typeStrings.empty())
            details.emplace_back("MessageTypes", Chroma::Join(", ", typeStrings.begin(), typeStrings.end()));

        details.emplace_back("MessageIdNumber", ToString(messageIdNumber));
        details.emplace_back("MessageIdName", messageIdName);

        const auto objectStrings = ObjectStrings(*pCallbackData);
        if (!objectStrings.empty())
            details.emplace_back("Objects", Chroma::Join(", ", objectStrings.begin(), objectStrings.end()));

        logger->Log(String(message), From(messageSeverity), details);

        return VK_FALSE;
    }

    bool Debug::ShouldSkip(const String& string)
    {
        return Chroma::Contains(string, "UNASSIGNED-CoreValidation-DrawState-InvalidImageLayout");
    }

    std::vector<String> Debug::ObjectStrings(const VkDebugUtilsMessengerCallbackDataEXT& data)
    {
        std::vector<String> strings;
        strings.reserve(data.objectCount);
        for (size_t i = 0; i < data.objectCount; ++i)
        {
            const auto objectInfo = From(data.pObjects[i]);
            strings.push_back(objectInfo.name + ":" + objectInfo.type);
        }

        return strings;
    }

    std::vector<String> Debug::TypeStrings(VkDebugUtilsMessageTypeFlagsEXT type)
    {
        const auto types = From(type);

        std::vector<String> strings;
        strings.reserve(types.size());
        for (auto& type : types)
            strings.push_back(From(type));

        return strings;
    }

    auto Debug::From(const VkDebugUtilsObjectNameInfoEXT& info) -> ObjectInfo
    {
        return
        {
            ToString(info.objectType),
            info.pObjectName ? String(info.pObjectName) : String()
        };
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

    auto Debug::From(VkDebugUtilsMessageTypeFlagsEXT type) -> std::vector<MessageType>
    {
        std::vector<MessageType> types;
        if (type & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT)
            types.push_back(MessageType::General);
        if (type & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
            types.push_back(MessageType::Validation);
        if (type & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
            types.push_back(MessageType::Performance);
        return types;
    }

    String Debug::From(MessageType type)
    {
        switch (type)
        {
        case MessageType::General:
            return "General";
        case MessageType::Validation:
            return "Validation";
        case MessageType::Performance:
            return "Performance";
        default:
            return "Unknown";
        }
    }
}