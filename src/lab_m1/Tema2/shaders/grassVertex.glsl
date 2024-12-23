#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texcoord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Time;

out vec3 frag_pos;
out vec2 frag_texcoord;
out vec3 frag_normal;
out vec3 frag_color;
out float noise_frag;
// 2D Random
float random (in vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))
                 * 43758.5453123);
}

// 2D Noise based on Morgan McGuire @morgan3d
// https://www.shadertoy.com/view/4dS3Wd
float noise (in vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    // Four corners in 2D of a tile
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    // Smooth Interpolation

    // Cubic Hermine Curve.  Same as SmoothStep()
    vec2 u = f*f*(3.0-2.0*f);
    // u = smoothstep(0.,1.,f);

    // Mix 4 coorners percentages
    return mix(a, b, u.x) +
            (c - a)* u.y * (1.0 - u.x) +
            (d - b) * u.x * u.y;
}

void main(){
    vec2 inp;
    inp.x = v_position.x;
    inp.y = v_position.z;

    // calculate the noise in that point
    float rez = noise(inp);

    // set new height
    frag_pos = v_position;
    frag_pos.y = rez - 1;


    vec3 colorLow = vec3(0.59, 0.24, 0.24);
    vec3 colorUp = vec3(0.89, 0.5, 0.21);
    frag_texcoord = v_texcoord;
    frag_normal = v_normal;
    frag_color = v_color;
    noise_frag = rez;
    gl_Position = Projection * View * Model * vec4(frag_pos, 1.0);
}