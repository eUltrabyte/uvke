#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
} ubo;

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoord;
// layout(location = 3) in vec3 normal;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragTexCoord;
// layout(location = 2) out vec3 fragNormal;

void main() {
    gl_PointSize = 1.0;
    gl_Position = ubo.projection * ubo.view * ubo.model * vec4(position, 1.0);
    fragColor = color;
    fragTexCoord = texCoord;
    // fragNormal = normalize(normal);
}