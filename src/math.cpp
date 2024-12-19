# include <math.hpp>
# include <iostream>
# include <cmath>

mat4	identityMatrix()
{
	mat4	matrix = {1, 0, 0, 0,
			  0, 1, 0, 0,
			  0, 0, 1, 0,
			  0, 0, 0, 1};
	return (matrix);
}

mat4	scaleMatrix(mat4 matrix, const vec3 &scaleFactor)
{
	mat4	scaleMatrix = {
		scaleFactor.entries[0], 0, 0, 0,
		0, scaleFactor.entries[1], 0, 0,
		0, 0, scaleFactor.entries[2], 0,
		0, 0, 0, 1
	};
	
	matrix = matrixMultiply(matrix, scaleMatrix);

	// printMatrix(matrix);
	return (matrix);
}

mat4	rotationMatrix(mat4 matrix, const vec3 &angles)
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
		sinf(y),  0,  cosf(z), 0,
		      0,  0,	    0, 1
	};
	mat4	zRotationMatrix = {
		cosf(z), -sinf(z), 0, 0,
		sinf(z),  cosf(z), 0, 0,
		      0,	0, 0, 0,
		      0,	0, 0, 1
	};

	matrix = matrixMultiply(matrix, xRotationMatrix);
	matrix = matrixMultiply(matrix, yRotationMatrix);
	matrix = matrixMultiply(matrix, zRotationMatrix);
	// printMatrix(matrix);
	return (matrix);
}

mat4	translationMatrix(mat4 matrix, const vec3 &factors)
{
	matrix.entries[12] *= factors.entries[0];
	matrix.entries[13] *= factors.entries[1];
	matrix.entries[14] *= factors.entries[2];
	// printMatrix(matrix);
	return (matrix);
}

vec4	vec4FromMat4Row(mat4 &matrix, unsigned int row)
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

vec4	vec4FromMat4collumn(mat4 &matrix, unsigned int collumn)
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

mat4	matrixMultiply(mat4 &matrix1, mat4 &matrix2)
{
	mat4	newMatrix = {0};
	
	newMatrix.entries[0] = dotProduct(vec4FromMat4Row(matrix1, 0), vec4FromMat4collumn(matrix2, 0));
	newMatrix.entries[1] = dotProduct(vec4FromMat4Row(matrix1, 1), vec4FromMat4collumn(matrix2, 0));
	newMatrix.entries[2] = dotProduct(vec4FromMat4Row(matrix1, 2), vec4FromMat4collumn(matrix2, 0));
	newMatrix.entries[3] = dotProduct(vec4FromMat4Row(matrix1, 3), vec4FromMat4collumn(matrix2, 0));
	
	newMatrix.entries[4] = dotProduct(vec4FromMat4Row(matrix1, 0), vec4FromMat4collumn(matrix2, 1));
	newMatrix.entries[5] = dotProduct(vec4FromMat4Row(matrix1, 1), vec4FromMat4collumn(matrix2, 1));
	newMatrix.entries[6] = dotProduct(vec4FromMat4Row(matrix1, 2), vec4FromMat4collumn(matrix2, 1));
	newMatrix.entries[7] = dotProduct(vec4FromMat4Row(matrix1, 3), vec4FromMat4collumn(matrix2, 1));
	
	newMatrix.entries[8] = dotProduct(vec4FromMat4Row(matrix1, 0), vec4FromMat4collumn(matrix2, 2));
	newMatrix.entries[9] = dotProduct(vec4FromMat4Row(matrix1, 1), vec4FromMat4collumn(matrix2, 2));
	newMatrix.entries[10] = dotProduct(vec4FromMat4Row(matrix1, 2), vec4FromMat4collumn(matrix2, 2));
	newMatrix.entries[11] = dotProduct(vec4FromMat4Row(matrix1, 3), vec4FromMat4collumn(matrix2, 2));
	
	newMatrix.entries[12] = dotProduct(vec4FromMat4Row(matrix1, 0), vec4FromMat4collumn(matrix2, 3));
	newMatrix.entries[13] = dotProduct(vec4FromMat4Row(matrix1, 1), vec4FromMat4collumn(matrix2, 3));
	newMatrix.entries[14] = dotProduct(vec4FromMat4Row(matrix1, 2), vec4FromMat4collumn(matrix2, 3));
	newMatrix.entries[15] = dotProduct(vec4FromMat4Row(matrix1, 3), vec4FromMat4collumn(matrix2, 3));
	
	printMatrix(newMatrix);
	return (newMatrix);
}

float	dotProduct(vec4 vector1, vec4 vector2)
{
	float	dot = 0;

	for (int i = 0; i < 4; i++)
		dot += vector1.entries[i] * vector2.entries[i];
	return (dot);
}

void	printMatrix(mat4 &matrix)
{
	std::cout << "| "<< matrix.entries[0] << ", " << matrix.entries[4] << ", " << matrix.entries[8] << ", " << matrix.entries[12] << " |" << std::endl;
	std::cout << "| "<< matrix.entries[1] << ", " << matrix.entries[5] << ", " << matrix.entries[9] << ", " << matrix.entries[13] << " |" << std::endl;
	std::cout << "| "<< matrix.entries[2] << ", " << matrix.entries[6] << ", " << matrix.entries[10] << ", " << matrix.entries[14] << " |" << std::endl;
	std::cout << "| "<< matrix.entries[3] << ", " << matrix.entries[7] << ", " << matrix.entries[11] << ", " << matrix.entries[15] << " |\n" << std::endl;
}
