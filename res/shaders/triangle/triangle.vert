#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;
layout (location = 2) in vec3 normal;

layout (location = 0) out vec3 frag_color;

void main()
{
	gl_Position = vec4(position, 1.0);
	frag_color = normal;
}