module;
#pragma once
#include <vulkan/vulkan.hpp>
#include "vk_mem_alloc.h"
#include "GLFW/glfw3.h"
export module VulkanContext;

export namespace Rendering::Vulkan
{
    struct Context
    {
        
        vk::Instance instance;
        vk::DebugUtilsMessengerEXT debug_callback;
        vk::Device device;
        vk::PhysicalDevice physical_device;
        vk::Queue graphics_queue;
        vk::SurfaceKHR surface;
        VmaAllocator allocator;
        int32_t graphics_queue_index = -1;
    };

    
}
