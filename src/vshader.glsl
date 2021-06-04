#version 330 core

layout(location = 0) in vec3 apos;
layout(location = 1) in vec3 acolor;
layout(location = 2) in vec2 atexcoord;

out vec3 vtxcolor;
out vec2 texcoord;

void main()
{
	gl_Position = vec4(apos.x, apos.y, apos.z, 1.0);
	vtxcolor = acolor;
	texcoord = atexcoord;
};
