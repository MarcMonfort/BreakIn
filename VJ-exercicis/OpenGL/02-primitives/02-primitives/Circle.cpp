#include <GL/glew.h>
#include <GL/gl.h>

#include "Circle.h"

#define M_PI 3.14159265358979323846



Circle* Circle::createCircle(float x, float y, float radius, int numberOfSides, ShaderProgram& program)
{
	Circle *circle = new Circle(x, y, radius, numberOfSides, program);

	return circle;
}


Circle::Circle(float x, float y, float radius, int numberOfSides, ShaderProgram& program)
{
	/* Assignando manualmente. 24 porque no he hecho TRIANGLE_FAN
	float vertices[24] = { x, y, x + radius, y, x, y + radius, 
						   x, y, x, y + radius, x - radius, y,
						   x, y, x - radius, y, x, y - radius,
						   x, y, x, y - radius, x + radius, y, };
	*/

	//int numberOfSides = 4;

	numberOfVertices = numberOfSides + 2;

	float twicePi = 2.0f * M_PI;


	float *circleVerticesX = new float[numberOfVertices];
	float *circleVerticesY = new float[numberOfVertices];

	circleVerticesX[0] = x;
	circleVerticesY[0] = y;

	for (int i = 1; i < numberOfVertices; i++)
	{
		circleVerticesX[i] = x + (radius * cos(i * twicePi / numberOfSides));
		circleVerticesY[i] = y + (radius * sin(i * twicePi / numberOfSides));
	}

	float *allCircleVertices = new float[(numberOfVertices) * 2];

	for (int i = 0; i < numberOfVertices; i++)
	{
		allCircleVertices[i * 2] = circleVerticesX[i];
		allCircleVertices[(i * 2) + 1] = circleVerticesY[i];
	}

	delete[] circleVerticesX;
	delete[] circleVerticesY;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 2*numberOfVertices * sizeof(float), allCircleVertices, GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2);

	delete[] allCircleVertices;

}

void Circle::render() const
{
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);  //triangle fan allows less vertices. 
}

void Circle::free()
{
	glDeleteBuffers(1, &vbo);
}

