/*!
 * \author David
 * \date 17-May-16.
 */

#include <cstdlib>
#include <algorithm>

#include <easylogging++.h>
#include "gl_shader_program.h"
#include "../../vulkan/render_context.h"

namespace nova {
    gl_shader_program::gl_shader_program(const shader_definition &source, const material_state& material, vk::RenderPass renderpass) : name(source.name) {
        device = render_context::instance.device;
        LOG(TRACE) << "Creating shader with filter expression " << source.filter_expression;
        filter = source.filter_expression;
        LOG(TRACE) << "Created filter expression " << filter;
        create_shader_module(source.vertex_source, vk::ShaderStageFlagBits::eVertex);
        LOG(TRACE) << "Creatd vertex shader";
        create_shader_module(source.fragment_source, vk::ShaderStageFlagBits::eFragment);
        LOG(TRACE) << "Created fragment shader";

        create_pipeline(renderpass, material);
    }

    gl_shader_program::gl_shader_program(gl_shader_program &&other) noexcept :
            name(std::move(other.name)), filter(std::move(other.filter)) {
    }

    void gl_shader_program::create_pipeline(vk::RenderPass pass, const material_state &material) {
        // Creates a pipeline out of compiled shaders

        std::vector<vk::PipelineShaderStageCreateInfo> stage_create_infos;

        vk::PipelineShaderStageCreateInfo vertex_create_info = {};
        vertex_create_info.stage = vk::ShaderStageFlagBits::eVertex;
        vertex_create_info.module = vertex_module;
        vertex_create_info.pName = "main";
        stage_create_infos.push_back(vertex_create_info);

        vk::PipelineShaderStageCreateInfo fragment_create_info = {};
        fragment_create_info.stage = vk::ShaderStageFlagBits::eFragment;
        fragment_create_info.module = fragment_module;
        fragment_create_info.pName = "main";
        stage_create_infos.push_back(fragment_create_info);

        // The vertex data is known by Nova. It just is. Each shader has inputs for all the vertex data because honestly
        // doing it differently is super hard. This will waste some VRAM but the number of vertices per chunk and
        // number of chunks will present a much easier win, especially since chunks are the big stuff and they will
        // always have all the vertex attributes
        vk::PipelineVertexInputStateCreateInfo vertex_stage_create_info = {};

        std::vector<vk::VertexInputBindingDescription> binding_descriptions;
        std::vector<vk::VertexInputAttributeDescription> attribute_descriptions;

        // Location in shader, buffer binding, data format, offset in buffer
        attribute_descriptions.emplace_back(0, 0, vk::Format::eR32G32B32Sfloat, 0);     // Position
        attribute_descriptions.emplace_back(1, 0, vk::Format::eR8G8B8A8Unorm,   12);    // Color
        attribute_descriptions.emplace_back(2, 0, vk::Format::eR32G32Sfloat,    16);    // UV
        attribute_descriptions.emplace_back(3, 0, vk::Format::eR16G16Unorm,     24);    // Lightmap UV
        attribute_descriptions.emplace_back(4, 0, vk::Format::eR32G32B32Sfloat, 32);    // Normal
        attribute_descriptions.emplace_back(5, 0, vk::Format::eR32G32B32Sfloat, 48);    // Tangent

        // Binding, stride, input rate
        binding_descriptions.emplace_back(0, 56, vk::VertexInputRate::eVertex);         // Position
        binding_descriptions.emplace_back(1, 56, vk::VertexInputRate::eVertex);         // Color
        binding_descriptions.emplace_back(2, 56, vk::VertexInputRate::eVertex);         // UV
        binding_descriptions.emplace_back(3, 56, vk::VertexInputRate::eVertex);         // Lightmap
        binding_descriptions.emplace_back(4, 56, vk::VertexInputRate::eVertex);         // Normal
        binding_descriptions.emplace_back(5, 56, vk::VertexInputRate::eVertex);         // Tangent

        vertex_stage_create_info.vertexAttributeDescriptionCount = static_cast<uint32_t>(attribute_descriptions.size());
        vertex_stage_create_info.pVertexAttributeDescriptions = attribute_descriptions.data();

        vertex_stage_create_info.vertexBindingDescriptionCount = static_cast<uint32_t>(binding_descriptions.size());
        vertex_stage_create_info.pVertexBindingDescriptions = binding_descriptions.data();

        vk::PipelineInputAssemblyStateCreateInfo input_assembly_create_info = {};
        input_assembly_create_info.topology = vk::PrimitiveTopology::eTriangleList;

        vk::Viewport viewport = {};
        viewport.x = 0;
        viewport.y = 0;
        viewport.width = material.output_width;
        viewport.height = material.output_height;

        vk::PipelineViewportStateCreateInfo viewport_create_info = {};
        viewport_create_info.scissorCount = 1;
        viewport_create_info.pViewports = &viewport;



    }

    void gl_shader_program::bind() noexcept {
        //glUseProgram(gl_name);
    }

    gl_shader_program::~gl_shader_program() {
        device.destroyShaderModule(vertex_module);
        device.destroyShaderModule(fragment_module);

        if(geometry_module) {
            device.destroyShaderModule(*geometry_module);
        }
        if(tessellation_evaluation_module) {
            device.destroyShaderModule(*tessellation_evaluation_module);
        }
        if(tessellation_control_module) {
            device.destroyShaderModule(*tessellation_control_module);
        }
    }

    void gl_shader_program::create_shader_module(const std::vector<uint32_t> &shader_source, vk::ShaderStageFlags flags) {
        vk::ShaderModuleCreateInfo create_info = {};
        create_info.codeSize = shader_source.size() * sizeof(uint32_t);
        create_info.pCode = shader_source.data();

        auto module = device.createShaderModule(create_info);

        if(flags == vk::ShaderStageFlagBits::eVertex) {
            vertex_module = module;
        } else if(flags == vk::ShaderStageFlagBits::eFragment) {
            fragment_module = module;
        }
    }

    std::string & gl_shader_program::get_filter() noexcept {
        return filter;
    }

    std::string &gl_shader_program::get_name() noexcept {
        return name;
    }

}
