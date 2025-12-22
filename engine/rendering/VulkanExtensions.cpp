module;
#include <vulkan/vulkan.hpp>

export module VulkanExtensions;
import std;

namespace Rendering::VulkanExtensions
{
    // Module state
    export bool has_debug_utils = false;

    // Debug callback for validation layers
    export VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(
        vk::DebugUtilsMessageSeverityFlagBitsEXT message_severity,
        vk::DebugUtilsMessageTypeFlagsEXT message_types,
        vk::DebugUtilsMessengerCallbackDataEXT const* callback_data,
        void* user_data)
    {
        const auto format_message = [&](std::string_view prefix) {
            return std::format("{} Validation Layer: {}: {}: {}", 
                callback_data->messageIdNumber,
                prefix,
                callback_data->pMessageIdName,
                callback_data->pMessage);
        };

        if (message_severity & vk::DebugUtilsMessageSeverityFlagBitsEXT::eError)
            std::cerr << format_message("Error") << '\n';
        else if (message_severity & vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning)
            std::cerr << format_message("Warning") << '\n';
        else if (message_severity & vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo)
            std::cout << format_message("Info") << '\n';
        else if (message_types & vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance)
            std::cout << format_message("Performance") << '\n';
        else if (message_severity & vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose)
            std::cout << format_message("Verbose") << '\n';

        return VK_FALSE;
    }

    // Validates that all required extensions are available
    export bool validateExtensions(
        const std::vector<const char*>& required,
        const std::vector<vk::ExtensionProperties>& available)
    {
        return std::ranges::all_of(required, [&](const char* ext_name) {
            const bool found = std::ranges::any_of(available, [&](const auto& prop) {
                return std::strcmp(prop.extensionName, ext_name) == 0;
            });

            if (!found)
                std::cerr << std::format("Error: Required extension not found: {}\n", ext_name);

            return found;
        });
    }

    // Adds debug messenger extension if available (debug builds only)
    export void addInstanceExtensions(std::vector<const char*>& extensions)
    {
        const auto available = vk::enumerateInstanceExtensionProperties();

#ifdef _DEBUG
        has_debug_utils = std::ranges::any_of(available, [](const auto& prop) {
            return std::strcmp(prop.extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == 0;
        });

        if (has_debug_utils) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        } else {
            std::cerr << std::format("{} is not available; disabling debug messenger\n",
                VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
#endif

        if (!validateExtensions(extensions, available))
            throw std::runtime_error("Required instance extensions are missing");
    }

    // Adds validation layers (debug builds only)
    export void addInstanceLayers(std::vector<const char*>& layers)
    {
#ifdef _DEBUG
        constexpr const char* validation_layer = "VK_LAYER_KHRONOS_validation";
        const auto available = vk::enumerateInstanceLayerProperties();

        const bool found = std::ranges::any_of(available, [](const auto& prop) {
            return std::strcmp(prop.layerName, "VK_LAYER_KHRONOS_validation") == 0;
        });

        if (found) {
            layers.push_back(validation_layer);
            std::cout << std::format("Enabled validation layer: {}\n", validation_layer);
        } else {
            std::cerr << std::format("Validation layer {} is not available\n", validation_layer);
        }
#endif
    }

    // Creates debug messenger info if available
    export std::optional<vk::DebugUtilsMessengerCreateInfoEXT> createDebugMessengerInfo()
    {
#ifdef _DEBUG
        if (has_debug_utils) {
            vk::DebugUtilsMessengerCreateInfoEXT info;
            info.setMessageSeverity(
                vk::DebugUtilsMessageSeverityFlagBitsEXT::eError |
                vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning);
            info.setMessageType(
                vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);
            info.setPfnUserCallback(debugCallback);
            return info;
        }
#endif
        return std::nullopt;
    }
}