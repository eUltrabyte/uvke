#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
} uniformBuffer;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texCoord;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragTexCoord;

void main() {
    gl_PointSize = 1.0;
    gl_Position = uniformBuffer.projection * uniformBuffer.view * uniformBuffer.model * vec4(position, 1.0);
    fragColor = vec4(color, 1.0);
    fragTexCoord = texCoord;
}