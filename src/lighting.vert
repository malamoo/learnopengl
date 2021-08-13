#version 330 core
layout(location = 0) in vec3 pos_attr;
layout(location = 1) in vec3 normal_attr;
layout(location = 2) in vec2 tex_coords_attr;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 frag_pos;
out vec3 normal;
out vec2 tex_coords;

void main()
{
    gl_Position = projection * view * model * vec4(pos_attr, 1.0);
    frag_pos = vec3(model * vec4(pos_attr, 1.0));
    normal = mat3(transpose(inverse(model))) * normal_attr;
    tex_coords = tex_coords_attr;
}
