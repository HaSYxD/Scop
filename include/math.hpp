# pragma once

typedef struct vec3 {
	float	entries[3];
}	vec3;

typedef struct vec4 {
	float	entries[4];
}	vec4;

typedef struct mat4 {
	float	entries[16];
}	mat4;

mat4	identityMatrix();
mat4	scaleMatrix(mat4 matrix, const vec3 &factors);
mat4	rotationMatrix(mat4 matrix, const vec3 &angles);
mat4	translationMatrix(mat4 matrix, const vec3 &factors);

mat4	createMatrixTransform(const vec3 translation);
mat4	createMatrixScale(const vec3 scale);
mat4	matrixMultiply(mat4 &matrix1, mat4 &matrix2);

float	dotProduct(vec4 vector1, vec4 vector2);
void	printMatrix(mat4 &matrix);
