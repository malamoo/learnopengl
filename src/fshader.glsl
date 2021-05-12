#version 330 core

in vec3 vtxcolor;

out vec4 fragcolor;

void main() 
{
        fragcolor = vec4(vtxcolor, 1.0);
}
