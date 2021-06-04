#version 330 core
out vec4 fragcolor;

in vec3 vtxcolor;
in vec2 texcoord;

uniform sampler2D tex;

void main() 
{
        fragcolor = texture(tex, texcoord);
};
