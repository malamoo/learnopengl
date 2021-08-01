#version 330 core
layout(location = 0) in vec3 posattr;
layout(location = 1) in vec2 texattr;

uniform mat4 transform;

out vec2 texcoord;

void main()
{
    gl_Position = transform * vec4(posattr, 1.0);
    texcoord = texattr;
}
