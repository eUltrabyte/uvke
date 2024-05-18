#version 450

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragNormal;

layout(location = 0) out vec4 outColor;

const vec4 sunColor = vec4(1.0, 1.0, 1.0, 1.0);
const vec3 sunDirection = normalize(vec3(0.0, 1.0, -1.0));

void main() {
    outColor = sunColor * max(0.0, dot(fragNormal, -sunDirection)) * fragColor * vec4(texture(texSampler, fragTexCoord).rgba);
    outColor = vec4(outColor.r, outColor.g, outColor.b, vec4(fragColor * texture(texSampler, fragTexCoord)).a);
}