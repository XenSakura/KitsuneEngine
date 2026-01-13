module;
#include <vulkan/vulkan.hpp>

export module VulkanSwapchain;
import ServiceLocator;
import VulkanContext;

namespace Rendering::Vulkan
{

    export struct Swapchain
    {
        vk::SwapchainKHR swapchain;
    	vk::Extent2D dimensions;
        vk::SurfaceFormatKHR surface_format;
        std::vector<vk::ImageView> image_views;
        std::vector<vk::Image> images;
    };

	export class SwapchainManager : public ISystem
	{
	private:
		const Vulkan::Context& vulkan_context;

		//temp until I need to make more
		Vulkan::Swapchain swapchain;
		
		struct swapchain_settings
		{
			vk::Extent2D extent;
			uint32_t swapchain_image_count;
			uint32_t selected_surface_format;
			std::vector<vk::SurfaceFormatKHR> surface_formats;
			vk::SurfaceTransformFlagBitsKHR pre_transform;
			vk::CompositeAlphaFlagBitsKHR composite;
		} swapchain_info;
		
		vk::SwapchainCreateInfoKHR createCreateInfo() const
		{
			vk::SwapchainCreateInfoKHR create_info = {};
			create_info.setSurface(vulkan_context.surface);								// Surface images will be presented to
			create_info.setMinImageCount(swapchain_info.swapchain_image_count);		// images in the swapchain 
			//TODO: Needs to change with whatever format we pick-- HDR or others
			create_info.setImageFormat(swapchain_info.surface_formats[swapchain_info.selected_surface_format].format);			// Format of the images
			create_info.setImageColorSpace(swapchain_info.surface_formats[swapchain_info.selected_surface_format].colorSpace);	// Color spaces of the images
			create_info.setImageExtent(swapchain_info.extent);						// resolution of the images
			create_info.setImageArrayLayers(1);										// 1 layer per image
			create_info.setImageUsage(vk::ImageUsageFlagBits::eColorAttachment); // the end result will be a colored image
			create_info.setImageSharingMode(vk::SharingMode::eExclusive);			// access mode of the images-- exclusive (Probably won't need to change?)
			create_info.setPreTransform(swapchain_info.pre_transform);				// Transform to apply to images
			create_info.setCompositeAlpha(swapchain_info.composite);								// Alpha blending to apply
			create_info.setPresentMode(vk::PresentModeKHR::eMailbox);				// FIFO = vsync, mailbox = flexible, immediate = screentearing
			create_info.setClipped(true);											// Clip obscured pixels (should always be true)
			return create_info;
		};

		Vulkan::Swapchain createVkSwapchain(const vk::SwapchainCreateInfoKHR& create_info, uint32_t width, uint32_t height) const
		{
			Vulkan::Swapchain swapchain;
			swapchain.images.reserve(swapchain_info.swapchain_image_count);
			swapchain.image_views.reserve(swapchain_info.swapchain_image_count);
			
			swapchain.swapchain = vulkan_context.device.createSwapchainKHR(create_info);
			swapchain.dimensions = VkExtent2D(width, height);
			swapchain.images = vulkan_context.device.getSwapchainImagesKHR(swapchain.swapchain);
			for (const auto& image : swapchain.images)
			{
				vk::ImageViewCreateInfo view_info = {};
				view_info.setImage(image);
				view_info.setViewType(vk::ImageViewType::e2D);
				//TODO: you know what
				view_info.setFormat(swapchain_info.surface_formats[swapchain_info.selected_surface_format].format);
				vk::ImageSubresourceRange viewImageSubResourceRange = {};
					viewImageSubResourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
					viewImageSubResourceRange.baseMipLevel = static_cast<unsigned>(0);
					viewImageSubResourceRange.levelCount = static_cast<unsigned>(1);
					viewImageSubResourceRange.baseArrayLayer = static_cast<unsigned>(0);
					viewImageSubResourceRange.layerCount = static_cast<unsigned>(1);
				view_info.setSubresourceRange(viewImageSubResourceRange);
				swapchain.image_views.emplace_back(vulkan_context.device.createImageView(view_info));
			};
			return swapchain;
		}
	public:
		//you have to initialize with the context
		SwapchainManager(const Vulkan::Context& context)
			:vulkan_context(context)
		{
			
			vk::SurfaceCapabilitiesKHR surface_properties = vulkan_context.physical_device.getSurfaceCapabilitiesKHR(vulkan_context.surface);

			swapchain_info.extent = vk::Extent2D(0, 0);

			// Grab Current surface extent
			if (surface_properties.currentExtent.width == 0xFFFFFFFF)
			{
				//TODO: temporary
				swapchain_info.extent.width = 1700;
				swapchain_info.extent.height = 900;
			}
			else
			{
				swapchain_info.extent = surface_properties.currentExtent;
			}

			//swapchain should always have 1 more than min image count of surface unless weird circumstances
			uint32_t swapchain_image_count = surface_properties.minImageCount + 1;
			//clamp image count 
			if (swapchain_image_count > surface_properties.maxImageCount)
				swapchain_image_count = surface_properties.maxImageCount;

			swapchain_info.swapchain_image_count = swapchain_image_count;
			
			//what formats can we use?
			swapchain_info.surface_formats = vulkan_context.physical_device.getSurfaceFormatsKHR(vulkan_context.surface);

			//TODO: Pick best format-- for HDR and such
			//TODO: TEMPORARY
			swapchain_info.selected_surface_format = 0;
			
			// find a surface transform
			
			if (surface_properties.supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity)
			{
				//use default
				swapchain_info.pre_transform = vk::SurfaceTransformFlagBitsKHR::eIdentity;
			}
			else
			{
				swapchain_info.pre_transform = surface_properties.currentTransform;
			}

			// one bitmask needs to be set according to the priority of presentation engine
			swapchain_info.composite = vk::CompositeAlphaFlagBitsKHR::eOpaque;
			if (surface_properties.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::eOpaque)
			{
				swapchain_info.composite = vk::CompositeAlphaFlagBitsKHR::eOpaque;
			}
			else if (surface_properties.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::eInherit)
			{
				swapchain_info.composite = vk::CompositeAlphaFlagBitsKHR::eInherit;
			}
			else if (surface_properties.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied)
			{
				swapchain_info.composite = vk::CompositeAlphaFlagBitsKHR::ePreMultiplied;
			}
			else if (surface_properties.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied)
			{
				swapchain_info.composite = vk::CompositeAlphaFlagBitsKHR::ePostMultiplied;
			}
		}
		SwapchainManager() = delete;
		SwapchainManager(const SwapchainManager&) = delete;
		SwapchainManager& operator=(const SwapchainManager&) = delete;
		
		~SwapchainManager()
		{
			destroySwapchain(swapchain);
		}
		
		// Only used for brand new swapchains
		Vulkan::Swapchain createSwapchain(uint32_t width, uint32_t height)
		{
			vk::SwapchainCreateInfoKHR create_info = createCreateInfo();
			create_info.setImageExtent(VkExtent2D(width, height));
			swapchain = createVkSwapchain(create_info, width, height);
			return swapchain;
		}
		// Used to resize swapchain
		Vulkan::Swapchain recreateSwapchain(uint32_t width, uint32_t height)
		{
			vulkan_context.device.waitIdle();
			vk::SwapchainCreateInfoKHR create_info = createCreateInfo();
			create_info.setImageExtent(VkExtent2D(width, height));
			vk::SwapchainKHR& old_swapchain = swapchain.swapchain;
			create_info.setOldSwapchain(old_swapchain);
			destroySwapchain(swapchain);
			swapchain = createVkSwapchain(create_info, width, height);
			return swapchain;
		}

		Vulkan::Swapchain getSwapchain()
		{
			return swapchain;
		}

		void destroySwapchain(Vulkan::Swapchain& swapchain) const
		{
			for (vk::ImageView& views : swapchain.image_views)
			{
				vulkan_context.device.destroyImageView(views);
			}
			swapchain.image_views.clear();
			vulkan_context.device.destroySwapchainKHR(swapchain.swapchain);	
		}
		
	};


}