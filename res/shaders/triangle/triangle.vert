#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;
layout (location = 2) in vec3 normal;

layout (location = 0) out vec3 frag_color;
layout (location = 1) out vec2 frag_texcoord;

layout (set = 0, binding = 0) uniform Transformation
{
	mat4 model;
	mat4 view;
	mat4 projection;
} trans;

layout (set = 1, binding = 1) uniform Speed
{
	float speed;
} sp;

void main()
{
	gl_Position = trans.projection * trans.view * trans.model * vec4(position, 1.0);
	frag_color = sin(sp.speed) * normal;
	frag_texcoord = texcoord;
}