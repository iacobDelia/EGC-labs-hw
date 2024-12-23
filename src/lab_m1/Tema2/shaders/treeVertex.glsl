#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texcoord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Time;
uniform float Radius;
// Output
out vec3 frag_pos;
out vec2 frag_texcoord;
out vec3 frag_normal;
out vec3 frag_color;



void main()
{
    float linearPos = abs(v_position.z) + abs(v_position.x);
    if(v_position.z < 0)
        linearPos += abs(v_position.z);
    if(v_position.x < 0)
        linearPos += abs(v_position.x);



    float add = sin(linearPos * Time) / 8;
    frag_pos = v_position;
    frag_pos.z += add;
    frag_pos.x += add;

    frag_texcoord = v_texcoord;
    frag_normal = v_normal;
    frag_normal.z += add;
    frag_normal.x += add;

    add *= 8;
    add = (add + 1)/2;
    vec3 colorCenter = vec3(0.8, 0.4, 0.13);
    vec3 colorOutside = vec3(0.7, 0.32, 0.19);
    
    frag_color = mix(colorCenter, colorOutside, add);

    gl_Position = Projection * View * Model * vec4(frag_pos, 1.0);
}