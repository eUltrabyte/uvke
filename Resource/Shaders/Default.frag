#version 450

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 fragTexCoord;
// layout(location = 2) in vec3 fragNormal;

layout(location = 0) out vec4 outColor;

/* layout(push_constant) uniform constants {
    bool hasTexture;
    bool dump1;
    bool dump2;
    bool dump3;
} PushConstants; */

const vec4 sunColor = vec4(1.0, 0.0, 0.0, 1.0);
const vec3 sunDirection = normalize(vec3(1.0, 1.0, -1.0));

void main() {
    // if(PushConstants.hasTexture) {
        // outColor =  sunColor * max(0.0, dot(fragNormal, -sunDirection)) * fragColor * vec4(texture(texSampler, fragTexCoord).rgba);
        outColor =  fragColor * vec4(texture(texSampler, fragTexCoord).rgba);
    /* } else {
        outColor = fragColor;
    } */

    /* if(outColor.w < 0.8) {
        discard;
    } */
}