# include <math.hpp>
# include <iostream>
# include <cmath>

// ----------------------------------------------------------------- //
// static helper fonctions for matrix multiplication and vectoe math //
// ----------------------------------------------------------------- //
static float	dotVec4(const vec4 &vector1, const vec4 &vector2)
{
	float	dot = 0;

	for (int i = 0; i < 4; i++)
		dot += vector1.entries[i] * vector2.entries[i];
	return (dot);
}

static float	dotVec3(const vec3 &v1, const vec3 &v2)
{
	return (v1.entries[0] * v2.entries[0]
		+ v1.entries[1] * v2.entries[1]
		+ v1.entries[2] * v2.entries[2]);
}

static vec3	crossVec3(vec3 const &u, const vec3 &v)
{
	return (vec3) {
		  u.entries[1] * v.entries[2] - u.entries[2] * v.entries[1],
		-(u.entries[0] * v.entries[2] - u.entries[2] * v.entries[0]),
		  u.entries[0] * v.entries[1] - u.entries[1] * v.entries[0]
	};
}

static vec3	normalizeVec3(const vec3 &v)
{
	float	mag = sqrt(dotVec3(v, v));
	
	vec3	nVector = {
		v.entries[0] / mag,
		v.entries[1] / mag,
		v.entries[2] / mag
	};
	return (nVector);
}

void	printMatrix(mat4 &matrix)
{
	std::cout << "| "<< matrix.entries[0] << ", " << matrix.entries[4] << ", " << matrix.entries[8] << ", " << matrix.entries[12] << " |" << std::endl;
	std::cout << "| "<< matrix.entries[1] << ", " << matrix.entries[5] << ", " << matrix.entries[9] << ", " << matrix.entries[13] << " |" << std::endl;
	std::cout << "| "<< matrix.entries[2] << ", " << matrix.entries[6] << ", " << matrix.entries[10] << ", " << matrix.entries[14] << " |" << std::endl;
	std::cout << "| "<< matrix.entries[3] << ", " << matrix.entries[7] << ", " << matrix.entries[11] << ", " << matrix.entries[15] << " |\n" << std::endl;
}

static vec4	vec4FromMat4Row(const mat4 &matrix, unsigned int row)
{
	vec4	newVector = {0};

	if (row > 3)
		return (newVector);

	newVector.entries[0] = matrix.entries[0 + row];
	newVector.entries[1] = matrix.entries[4 + row];
	newVector.entries[2] = matrix.entries[8 + row];
	newVector.entries[3] = matrix.entries[12 + row];
	return (newVector);
}

static vec4	vec4FromMat4collumn(const mat4 &matrix, unsigned int collumn)
{
	vec4	newVector = {0};

	if (collumn > 3)
		return (newVector);

	newVector.entries[0] = matrix.entries[0 + (4 * collumn)];
	newVector.entries[1] = matrix.entries[1 + (4 * collumn)];
	newVector.entries[2] = matrix.entries[2 + (4 * collumn)];
	newVector.entries[3] = matrix.entries[3 + (4 * collumn)];
	return (newVector);
}

// ------------------------------------------------------------------------ //
// core math fonctions for matrix creation, multiplication, and vector math //
// ------------------------------------------------------------------------ //

mat4	identityMatrix()
{
	mat4	matrix = {1, 0, 0, 0,
			  0, 1, 0, 0,
			  0, 0, 1, 0,
			  0, 0, 0, 1};
	return (matrix);
}

mat4	scaleMatrix(const vec3 &scaleFactor)
{
	mat4	scaleMatrix = {
		scaleFactor.entries[0], 0, 0, 0,
		0, scaleFactor.entries[1], 0, 0,
		0, 0, scaleFactor.entries[2], 0,
		0, 0, 0, 1
	};

	return (scaleMatrix);
}

mat4	rotationMatrix(const vec3 &angles)
{
	float	x = angles.entries[0];
	float	y = angles.entries[1];
	float	z = angles.entries[2];

	mat4	xRotationMatrix = {
		1, 	  0, 	   0, 0,
		0,  cosf(x), sinf(x), 0,
		0, -sinf(x), cosf(x), 0,
		0,	  0,	   0, 1
	};
	mat4	yRotationMatrix = {
		 cosf(y),  0, -sinf(y), 0,
		       0,  1,	    0, 0,
		 sinf(y),  0, cosf(y), 0,
		       0,  0,	    0, 1
	};
	mat4	zRotationMatrix = {
		 cosf(z), sinf(z), 0, 0,
		-sinf(z), cosf(z), 0, 0,
		       0,	0, 1, 0,
		       0,	0, 0, 1
	};

	mat4	newMatrix = identityMatrix();

	newMatrix = matrixMultiply(newMatrix, xRotationMatrix);
	newMatrix = matrixMultiply(newMatrix, yRotationMatrix);
	newMatrix = matrixMultiply(newMatrix, zRotationMatrix);
	// printMatrix(matrix);
	return (newMatrix);
}

mat4	translationMatrix(const vec3 &factors)
{
	mat4	newMatrix = identityMatrix();	

	newMatrix.entries[12] = factors.entries[0];
	newMatrix.entries[13] = factors.entries[1];
	newMatrix.entries[14] = factors.entries[2];
	return (newMatrix);
}

mat4	viewMatrix(const vec3 from, const vec3 to)
{
	vec3	worldUp = {0.0, 0.0, -1.0};
	vec3	fVector = {
		to.entries[0] - from.entries[0],
		to.entries[1] - from.entries[1],
		to.entries[2] - from.entries[2]
	};
	fVector = normalizeVec3(fVector);

	vec3	rVector = normalizeVec3(crossVec3(fVector, worldUp));
	vec3	uVector = normalizeVec3(crossVec3(rVector, fVector));

	return (mat4) {
		     rVector.entries[0],      uVector.entries[0],    -fVector.entries[0], 0,
		     rVector.entries[1],      uVector.entries[1],    -fVector.entries[1], 0,
		     rVector.entries[2],      uVector.entries[2],    -fVector.entries[2], 0,
		-dotVec3(rVector, from), -dotVec3(uVector, from), dotVec3(fVector, from), 1
	};
}

mat4	matrixMultiply(const mat4 &matrix1, const mat4 &matrix2)
{
	mat4	newMatrix = identityMatrix();
	int	k = 0;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			newMatrix.entries[k++] = dotVec4(vec4FromMat4Row(matrix1, j), vec4FromMat4collumn(matrix2, i));
	return (newMatrix);
}

mat4	projectionMatrix(float fovy, const float aspect, float near, float far)
{
	fovy = fovy *  M_PI / 360.0f;	// convert fov-y to radians and divid it by two
	
	float	t = 1.0f / tanf(fovy);
	float	n = near;
	float	f = far;
	
	mat4	newMatrix = {
		t / aspect, 0,			 0,  0,
			 0, t,			 0,  0,
			 0, 0,   (n + f) / (n - f), -1,
			 0, 0, 2 * f * n / (n - f),  0
	};
	return (newMatrix);
}

vec3	getCameraDirection(const float &yaw, const float &pitch)
{
	vec3	dir = {
		cosf(yaw) * cosf(pitch),
		sinf(yaw) * cosf(pitch),
		sinf(pitch)
	};

	normalizeVec3(dir);
	return (dir);
}
