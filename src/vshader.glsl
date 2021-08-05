#version 330 core
layout(location = 0) in vec3 posattr;
layout(location = 1) in vec2 texattr;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 texcoord;

void main()
{
    gl_Position = projection * view * model * vec4(posattr, 1.0);
    texcoord = texattr;
}
