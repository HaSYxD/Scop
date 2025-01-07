#version 420 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNorm;
layout (location = 3) in vec3 aColIndex;

out vec3	Norm;
out vec3	FragPos;
out vec2	Tex;
out vec3	ColIndex;

uniform mat4	model;
uniform mat4	view;
uniform mat4	projection;

void	main()
{
	vec4	pos = model * vec4(aPos, 1.0f);
	
	gl_Position = projection * view * model * pos;
	Tex = vec2(aTex.x, aTex.y * -1);
	Norm = aNorm;
	FragPos = vec3(pos);
	ColIndex = aColIndex;
}
