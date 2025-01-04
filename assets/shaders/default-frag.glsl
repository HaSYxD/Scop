#version 330 core

in vec3	Norm;
in vec3	FragPos;
in vec2	Tex;

out vec4	FragColor;

uniform float	time;
uniform vec3	lPos;

uniform vec3	ambColor;
uniform vec3	diffColor;
uniform vec3	specColor;
uniform float	specExponent;

void	main()
{

	float	ambiantStrength = 0.1;
	vec3	ambiant = ambiantStrength * ambColor;

	vec3	lDir = normalize(lPos - FragPos);

	float	diff = max(dot(Norm, lDir), 0.0);
	vec3	diffuse = diff * diffColor;

	FragColor = vec4((ambiant + diffuse), 1.0f);
	//-FragColor = vec4(Norm, 1.0f);
}
