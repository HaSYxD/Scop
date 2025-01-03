#version 330 core

in vec3	Norm;
in vec3	FragPos;
in vec2	Tex;

out vec4	FragColor;

uniform float	time;
uniform vec3	lPos;

void	main()
{
	vec3	lColor = vec3(1.0, 1.0, 1.0);
	vec3	objColor = vec3(1.0, 0.0f, 0.0f);

	float	ambiantStrength = 0.1;
	vec3	ambiant = ambiantStrength * lColor;

	vec3	lDir = normalize(lPos - FragPos);

	float	diff = max(dot(Norm, lDir), 0.0);
	vec3	diffuse = diff * lColor;

	FragColor = vec4((ambiant + diffuse) * objColor, 1.0f);
	//-FragColor = vec4(Norm, 1.0f);
}
