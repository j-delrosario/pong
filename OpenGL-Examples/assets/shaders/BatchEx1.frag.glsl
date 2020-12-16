#version 450 core

layout (location = 0) out vec4 o_Color;

uniform vec4 u_Color = vec4(1.0f, 1.0f, 1.0f, 1.0f);

void main()
{
	o_Color = u_Color;
}