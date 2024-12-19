#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;

out vec3	vColors;

uniform mat4	translation;
uniform mat4	scale;
uniform mat4	model;

void	main()
{
	vColors = aCol;
	gl_Position = model * vec4(aPos, 1.0f);
}
