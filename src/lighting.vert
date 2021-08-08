#version 330 core
layout(location = 0) in vec3 pos_attr;
layout(location = 1) in vec3 norm_attr;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 frag_pos;
out vec3 norm;

void main()
{
    gl_Position = projection * view * model * vec4(pos_attr, 1.0);
    frag_pos = vec3(model * vec4(pos_attr, 1.0));
    norm = mat3(transpose(inverse(model))) * norm_attr;
}
