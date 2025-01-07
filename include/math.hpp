# pragma once

typedef struct vec2 {
	float entries[2];
}	vec2;

typedef struct vec3 {
	float	entries[3];
}	vec3;

typedef struct vec4 {
	float	entries[4];
}	vec4;

typedef struct mat4 {
	float	entries[16];
}	mat4;

// Create a simple identity matrix, the base for the construction of any other matrices.
mat4	identityMatrix();

// Scale the given matrix by multiplying it with the given factor.
// Should be done first in the case of the creation of a model transform.
mat4	scaleMatrix(const vec3 &factors);

// Rotate the given matrix by multiplying it with the given angles.
// Should be done after the scaling and before the translation in the case of the creation of a model transform.
mat4	rotationMatrix(const vec3 &angles);

// Translate the given matrix by multipying it by the factors.
mat4	translationMatrix(const vec3 &factors);

// Multiply the first given matrix by the second given matrix.
mat4	matrixMultiply(const mat4 &matrix1, const mat4 &matrix2);

// Create a view transform from a camera position vector and a looking point vector.
mat4	viewMatrix(const vec3 from, const vec3 to);

// Create a perspective projection transform.
mat4	projectionMatrix(float fovy, const float aspect, float near, float far);

vec3	getCameraDirection(const float &yaw, const float &pitch);
