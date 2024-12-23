#version 330
// Input
in vec3 frag_pos;
in vec3 frag_normal;
in vec2 frag_texcoord;
in vec3 frag_color;
in float noise_frag;
// Output
layout(location = 0) out vec4 out_color;

void main()
{

    vec3 colorLow = vec3(0.59, 0.24, 0.24);
    vec3 colorUp = vec3(0.89, 0.5, 0.21);
    out_color = vec4(mix(colorLow, colorUp, noise_frag), 1);
}