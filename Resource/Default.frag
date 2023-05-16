#version 450

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = fragColor * vec4(texture(texSampler, fragTexCoord).rgb, 1.0);
    // outColor = fragColor * vec4(texture(texSampler, fragTexCoord).rgba);

    if(outColor.w < 0.8) {
        discard;
    }
}