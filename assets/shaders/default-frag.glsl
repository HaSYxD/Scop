#version 420 core

in vec3	Norm;
in vec3	FragPos;
in vec2	Tex;
in vec3 ColIndex;

out vec4	FragColor;

uniform float	time;
uniform vec3	lPos;

uniform vec3	ambColor;
uniform vec3	diffColor;
uniform vec3	specColor;
uniform float	specExponent;
uniform vec3	ratio;
uniform sampler2D	mat;
uniform int	texMissing;

vec3	colors[15] = {
	vec3(1.0f, 1.0f, 1.0f),
	vec3(1.0f, 0.0f, 1.0f),
	vec3(1.0f, 1.0f, 0.0f),
	vec3(0.0f, 1.0f, 1.0f),
	vec3(0.0f, 0.0f, 1.0f),
	vec3(0.0f, 1.0f, 0.0f),
	vec3(1.0f, 0.0f, 0.0f),
	vec3(0.0f, 0.0f, 0.0f),
	vec3(0.5f, 0.5f, 0.5f),
	vec3(0.5f, 0.0f, 0.5f),
	vec3(0.5f, 0.5f, 0.0f),
	vec3(0.0f, 0.5f, 0.5f),
	vec3(0.0f, 0.0f, 0.5f),
	vec3(0.0f, 0.5f, 0.0f),
	vec3(0.5f, 0.0f, 0.0f),
};

void	main()
{
	float	ambiantStrength = 0.1;
	vec3	ambiant = ambiantStrength * ambColor;

	vec3	lDir = normalize(lPos - FragPos);

	float	diff = max(dot(Norm, lDir), 0.0);
	vec3	diffuse = diff * diffColor;

	vec4	finalColors[3] = {	
		(vec4(texMissing) + texture(mat, Tex)) * vec4(ambiant + diffuse, 1.0f),
		vec4(Norm , 1.0f),
		vec4(colors[int(ColIndex) % 15], 1.0f)
	};
	FragColor = (ratio.x * finalColors[0])
		+ (ratio.y * finalColors[1])
		+ (ratio.z * finalColors[2]);
}
