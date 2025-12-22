 module;
// #include "vulkan/vulkan.hpp"
// #define VMA_IMPLEMENTATION
// #include "vk_mem_alloc.h"
//
// import VulkanContext;
 export module RenderTargetManager;
//
// namespace Rendering
// {
//
//     
//     struct RenderTarget
//     {
//         //bad way to resolve what this is-- fix later
//         const char* name; 
//         vk::Image image;
//         VmaAllocation allocation;
//         vk::ImageView view;
//     
//         // Properties (for using the resource)
//         vk::Format format;
//         vk::Extent2D extent;
//     
//         // State tracking (for automatic barriers)
//         vk::ImageLayout current_layout = vk::ImageLayout::eUndefined;
//     };
//     
//     class RenderTargetManager
//     {
//     private:
//         RenderTarget draw_color_target;
//         RenderTarget draw_depth_target;
//         RenderTarget createRenderTarget(vk::Format format, vk::Extent2D size, vk::ImageUsageFlags usage)
//         {
//             
//             vk::ImageCreateInfo image_info = {};
//             image_info.usage = usage;
//             image_info.format = format;
//             image_info.extent = VkExtent3D{size.width, size.height, 1};
//
//             VmaAllocationCreateInfo alloc_info = {};
//             alloc_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;
//             alloc_info.flags = VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
//             
//             
//             RenderTarget t;
//             
//             t.format = format;
//             t.extent = size;
//             VkImage local_image;
//             vmaCreateImage(parent.allocator, image_info, &alloc_info, &local_image, &t.allocation, nullptr);
//             t.image = local_image;
//             // Your creation code here
//         }
//         VulkanContext& parent;
//     public:
//         RenderTargetManager() = delete;
//         RenderTargetManager(VulkanContext& parent) : parent(parent) {};
//         
//         RenderTarget& createDrawColorTarget()
//         {
//             vk::ImageUsageFlags drawImageUsages{};
//             drawImageUsages |= vk::ImageUsageFlags::BitsType::eTransferSrc;
//             drawImageUsages |= vk::ImageUsageFlags::BitsType::eTransferDst;
//             drawImageUsages |= vk::ImageUsageFlags::BitsType::eStorage;
//             drawImageUsages |= vk::ImageUsageFlags::BitsType::eColorAttachment;
//             
//             vk::ImageCreateInfo image_info = {};
//             image_info.usage = drawImageUsages;
//             image_info.format = vk::Format::eR16G16B16A16Sfloat;
//             image_info.extent = VkExtent3D{size.width, size.height, 1};
//             
//             draw_color_target.format = vk::Format::eR16G16B16A16Sfloat;
//             draw_color_target.extent = VkExtent2D{1, 0, 0};
//             
//             
//             return draw_color_target;
//         }
//     
//         RenderTarget& createDepthTarget()
//         {
//             vk::ImageUsageFlags depthImageUsages{};
//             depthImageUsages |= vk::ImageUsageFlags::BitsType::eTransferSrc;
//             depthImageUsages |= vk::ImageUsageFlags::BitsType::eTransferDst;
//             depthImageUsages |= vk::ImageUsageFlags::BitsType::eStorage;
//             depthImageUsages |= vk::ImageUsageFlags::BitsType::eDepthStencilAttachment;
//             vk::ImageCreateInfo image_info = {};
//             image_info.usage = depthImageUsages;
//             image_info.format = vk::Format::eR16G16B16A16Sfloat;
//             image_info.extent = VkExtent3D{size.width, size.height, 1};
//         }
//     };
// }