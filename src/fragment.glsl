#version 330 core
in vec2 texcoord;

uniform sampler2D tex1;
uniform sampler2D tex2;

out vec4 color;

void main()
{
    color = mix(texture(tex1, texcoord), texture(tex2, texcoord), 0.2);
}
