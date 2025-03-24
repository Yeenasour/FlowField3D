#include <Axes.h>
#include <iostream>

Axes::Axes(float len) : length(len)
{
	initAxes();
	updateModelMatrix();
}

Axes::~Axes()
{
	glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Axes::initAxes()
{
	float vertices[] = {
		-1.5f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,
		 1.5f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,
		 0.0f, -1.5f,  0.0f, 0.0f, 1.0f, 0.0f,
		 0.0f,  1.5f,  0.0f, 0.0f, 1.0f, 0.0f,
		 0.0f,  0.0f, -1.5f, 0.0f, 0.0f, 1.0f,
		 0.0f,  0.0f,  1.5f, 0.0f, 0.0f, 1.0f
	};

	unsigned int indicies[] = {
		0, 1,
		2, 3,
		4, 5
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Axes::updateModelMatrix()
{
	glm::mat4 mat = glm::mat4(1.0f);
	this->modelMatrix = glm::scale(mat, glm::vec3(this->length));
}

void Axes::setLength(float len)
{
	this->length = len;
}

void Axes::Draw(Camera &camera, Shader &shaderProgram)
{
	shaderProgram.use();
	glm::mat4 VP = camera.getViewProjectionMatrix();
	glm::mat4 MVP = VP * modelMatrix;

	shaderProgram.setUniform4fv("modelViewProjectionMatrix", &MVP[0][0]);

	glBindVertexArray(VAO);
	glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

