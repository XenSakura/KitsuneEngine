module;
#include <vulkan/vulkan.hpp>

export module VulkanPipeline;
import ServiceLocator;
import VulkanContext;
import RenderTargetManager;
namespace Rendering::Vulkan
{
    export class PipelineManager : public ISystem
    {
    public:
        PipelineManager() = delete;

        PipelineManager(const Context& context)
	        :m_context(context)
        {
            
        }

    	vk::Pipeline buildPipeline()
        {
	        return vk::Pipeline();
        }
        
        ~PipelineManager()
        {
            
        }

    	class PipelineBuilder
        {
        private:
	        std::vector<vk::PipelineShaderStageCreateInfo> m_shaderStages;

        	vk::PipelineInputAssemblyStateCreateInfo m_inputAssembly;
			vk::PipelineRasterizationStateCreateInfo m_rasterization;
        	vk::PipelineDepthStencilStateCreateInfo m_depthStencil;
        	vk::PipelineColorBlendAttachmentState m_colorBlendAttachment;
        	vk::PipelineMultisampleStateCreateInfo m_multisample;
        	vk::PipelineRenderingCreateInfo m_pipeline_info;
        	
        	vk::Format colorFormat;
        public:
        	PipelineBuilder() {};
        	SetPipelineType()
        	
        } m_pipeline_builder;
    private:
        void initialize()
        {
	        vk::PushConstantRange push_constant_range =
	        {
	        	.offset = 0,
		        .stageFlags = vk::ShaderStageFlagBits::eVertex,
		        .size = static_cast<uint32_t>(sizeof(vk::DeviceAddress))
	        };

	        vk::PipelineLayoutCreateInfo pipeline_layout_info =
	        {
		        .setLayoutCount = 1,
		        // Put descriptor set layout here
		        .pSetLayouts = 0,
		        .pushConstantRangeCount = 1,
		        .pPushConstantRanges = &push_constant_range
	        };

	        vk::PipelineLayout pipeline = m_context.device.createPipelineLayout(pipeline_layout_info);

	        vk::VertexInputBindingDescription vertex_binding =
	        {
		        .binding = static_cast<uint32_t>(sizeof(0)),
	        	//TODO: vertex struct
		        .stride = static_cast<uint32_t>(sizeof(int)),
		        .inputRate = vk::VertexInputRate::eVertex
	        };

	        std::vector<vk::VertexInputAttributeDescription> vertex_attributes =
	        {
		        {.location = (uint32_t)0, .binding = (uint32_t)0, .format = vk::Format::eR32G32B32Sfloat},
		        {.location = (uint32_t)1, .binding = (uint32_t)0, .format = vk::Format::eR32G32B32Sfloat, .offset = (uint32_t)4},
		        {.location = (uint32_t)0, .binding = (uint32_t)0, .format = vk::Format::eR32G32Sfloat, .offset = (uint32_t)6}
	        };

	        vk::PipelineVertexInputStateCreateInfo vertex_input_state =
	        {
		        .vertexBindingDescriptionCount = 1,
		        .pVertexBindingDescriptions = &vertex_binding,
		        .vertexAttributeDescriptionCount = static_cast<uint32_t>(vertex_attributes.size()),
		        .pVertexAttributeDescriptions = vertex_attributes.data()
	        };


	        vk::PipelineInputAssemblyStateCreateInfo input_assembly_state = {.topology = vk::PrimitiveTopology::eTriangleList};
	        std::vector<vk::PipelineShaderStageCreateInfo> shader_stages = {};

	        vk::PipelineShaderStageCreateInfo pipeline_shader_stage =
	        { .stage = vk::ShaderStageFlagBits::eVertex,
		        .module = vk::ShaderModule(),
		        //entry function of each shader
		        .name = "main"
	        };

	        vk::PipelineShaderStageCreateInfo pipeline_fragment_shader_stage =
	        {
		        .stage = vk::ShaderStageFlagBits::eFragment,
		        .module = vk::ShaderModule(),
		        .name = "main"
	        };

	        vk::PipelineViewportStateCreateInfo viewport_state =
	        {
		        .viewportCount = 1,
		        .scissorCount = 1
	        };

	        std::vector<vk::DynamicState> dynamic_states =
	        {
		        vk::DynamicState::eViewport,
		        vk::DynamicState::eScissor
	        };

	        vk::PipelineDynamicStateCreateInfo dynamic_state =
	        {
		        .dynamicStateCount = 2,
		        .pDynamicStates = dynamic_states.data()
	        };

	        vk::PipelineDepthStencilStateCreateInfo depth_stencil_state =
	        {
		        .depthTestEnable = VK_TRUE,
		        .depthWriteEnable = VK_TRUE,
		        .depthCompareOp = vk::CompareOp::eLessOrEqual
	        };
        	
	        vk::PipelineRenderingCreateInfo rendering_create_info ={};
	        // unlock render target manager asap
	        {
		        auto manager = ServiceLocator::Instance()->Get<Vulkan::RenderTargetManager>().lock();
        		
		        rendering_create_info.colorAttachmentCount = 1;
		        rendering_create_info.pColorAttachmentFormats = &manager->getColorRenderTarget().format;
		        rendering_create_info.depthAttachmentFormat = manager->getDepthRenderTarget().format;
	        }
        		
        	

	        vk::PipelineColorBlendAttachmentState blend_attachment_state =
	        {
		        .colorWriteMask = static_cast<vk::Bool32>(0xF),
	        };

	        vk::PipelineColorBlendStateCreateInfo color_blend_state =
	        {
		        .attachmentCount = 1,
		        .pattachments = &blend_attachment_state
	        };

	        vk::PipelineRasterizationStateCreateInfo rasterization_state =
	        {
		        .lineWidth = 1.0f
	        };

	        vk::PipelineMultisampleStateCreateInfo ms_state =
	        {
		        .rasterizationSamples = vk::SampleCountFlagBits::e1,
	        };

	        vk::GraphicsPipelineCreateInfo pipeline_create_info =
	        {
		        .pNext = &rendering_create_info,
	        	// stages, e.g compute, or others
		        .stageCount = 2,
	        	// shader stages
		        .pStages = shader_stages.data(),
	        	//can be vertex or mesh shader
		        .pVertexInputState = &vertex_input_state,
	        	//triangles, lines, other primitives
		        .pInputAssemblyState = &input_assembly_state,
	        	// # of viewports and scissors
		        .pViewportState = &viewport_state,
	        	//
		        .pRasterizationState = &rasterization_state,
		        .pMultisampleState = &ms_state,
		        .pDepthStencilState = &depth_stencil_state,
		        .pColorBlendState = &color_blend_state,
		        .pDynamicState = &dynamic_state,
		        .layout = pipeline_layout_info
	        };
        };

    	const Context& m_context;
        std::vector<vk::Pipeline> m_pipelines;
        std::vector<vk::PipelineLayout> m_pipelineLayout;
    };
    
}
