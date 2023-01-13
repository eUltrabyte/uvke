#version 450

/* layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
} uniformBuffer; */

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

layout(location = 0) out vec4 fragColor;

void main() {
    gl_Position = /* uniformBuffer.projection * uniformBuffer.view * uniformBuffer.model * */ vec4(position, 1.0);
    fragColor = color;
}