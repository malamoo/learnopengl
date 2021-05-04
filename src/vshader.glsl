#version 330 core

layout (location = 0) in vec3 apos;
layout (location = 1) in vec3 acolor;

out vec3 vtxcolor;

void main()
{
        gl_Position = vec4(apos.x, apos.y, apos.z, 1.0);
        vtxcolor = acolor; 
};
