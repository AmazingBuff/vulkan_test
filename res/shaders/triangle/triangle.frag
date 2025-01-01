#version 460

layout (location = 0) in vec3 frag_color;
layout (location = 1) in vec2 frag_texcoord;
layout (location = 0) out vec4 color;

layout (set = 1, binding = 0) uniform sampler2D sampler_s;

void main()
{
	color = texture(sampler_s, frag_texcoord);
}