#version 450

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

/* layout(push_constant) uniform constants {
    bool hasTexture;
    bool dump1;
    bool dump2;
    bool dump3;
} PushConstants; */

void main() {
    // if(PushConstants.hasTexture) {
        outColor = fragColor * vec4(texture(texSampler, fragTexCoord).rgba);
    /* } else {
        outColor = fragColor;
    } */

    /* if(outColor.w < 0.8) {
        discard;
    } */
}