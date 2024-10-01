#version 330 core

in vec3		vColors;

out vec4	FragColor;

uniform float	time;

void	main()
{
	FragColor = vec4(vec3(sin(time)) * vColors + 0.5f, 1.0f);
}
