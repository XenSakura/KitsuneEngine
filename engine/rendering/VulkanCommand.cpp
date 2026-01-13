module;

#include <vulkan/vulkan.hpp>
export module VulkanCommand;
import std;
import VulkanContext;
import ServiceLocator;

namespace Rendering::Vulkan
{
    //TODO: I think we want the ability to record across multiple threads maybe?
    export class CommandPoolManager : public ISystem
    {
    public:
        CommandPoolManager(const Vulkan::Context& context)
            :m_context(context)
        {
            command_pool_info = vk::CommandPoolCreateInfo();
            command_pool_info.queueFamilyIndex = context.graphics_queue_index;	
        }
        ~CommandPoolManager()
        {
            for (uint32_t i = 0; i < FRAMES; i++)
            {
                m_context.device.destroyCommandPool(command_pools[i]);
            }
        }
        CommandPoolManager() = delete;
        CommandPoolManager(const CommandPoolManager&) = delete;
        CommandPoolManager& operator=(const CommandPoolManager&) = delete;
        CommandPoolManager(CommandPoolManager&&) = delete;
        CommandPoolManager& operator=(CommandPoolManager&&) = delete;

        void BuildCommandPools()
        {
            for (uint32_t i = 0; i < FRAMES; ++i)
                {
                    command_pools[i] = m_context.device.createCommandPool(command_pool_info, nullptr);
					
                    vk::CommandBufferAllocateInfo command_buffer_info = {};
                    command_buffer_info.commandPool = command_pools[i];
                    command_buffer_info.commandBufferCount = 1;
                    command_buffer_info.level = vk::CommandBufferLevel::ePrimary;

                    //just grab the first for now
                    command_buffers[i] = m_context.device.allocateCommandBuffers(command_buffer_info)[0];
                }
        }

        vk::CommandBuffer& GetCommandBuffer(uint32_t frameIndex)
        {
            assert(frameIndex > FRAMES && "frameIndex is greater than the amount of frames possible.");
            return command_buffers[frameIndex];
        }
    private:
        const Vulkan::Context& m_context;
        //shared command pool allocation info
        vk::CommandPoolCreateInfo command_pool_info;

        vk::CommandPool command_pools[FRAMES];
        vk::CommandBuffer command_buffers[FRAMES];
    };

    
}

