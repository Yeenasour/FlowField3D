#include <VectorFieldRenderer.h>

VectorFieldRenderer::VectorFieldRenderer(VectorField &field, int density, int segments) 
	: vf(field), vectorDensity(density), segments(segments), numIndecies(2*(segments)*pow(density, 3))
{
	initBuffers();
}

VectorFieldRenderer::~VectorFieldRenderer()
{
	glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void VectorFieldRenderer::initBuffers()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	updateBuffers();
	glBindVertexArray(0);
}

void VectorFieldRenderer::updateBuffers()
{
	int vecNum = pow(vectorDensity, 3);
	int totalVerts = vecNum*2*(this->segments + 1);
	std::vector<glm::vec3> basePoints;
	basePoints.reserve(vecNum);
	for (int i = 0; i < vectorDensity; i++)
	{
		for (int j = 0; j < vectorDensity; j++)
		{
			for (int k = 0; k < vectorDensity; k++)
			{
				float x = -1.0f + i * (2.0f / (vectorDensity - 1));
				float y = -1.0f + j * (2.0f / (vectorDensity - 1));
				float z = -1.0f + k * (2.0f / (vectorDensity - 1));
				basePoints.emplace_back(x, y, z);
			}
		}
	}
	std::vector<glm::vec3> vertecies;
	vertecies.reserve(totalVerts);
	for (int i = 0; i < vecNum; i++)
	{
		glm::vec3 vert = basePoints.at(i);
		for (int j = 0; j < (this->segments + 1); j++)
		{
			vertecies.push_back(vert);
			glm::vec3 color = glm::vec3((1.0 - ((float)j / 5)));
			vertecies.push_back(color);
			vert = vert + 0.1f * (vf.evalAt(vert.x, vert.y, vert.z));
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, totalVerts*sizeof(glm::vec3), vertecies.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)sizeof(glm::vec3));
	glEnableVertexAttribArray(1);

	std::vector<unsigned int> indices;
	indices.reserve(numIndecies);
	for (unsigned int i = 0; i < vecNum; i++)
	{
		unsigned int baseIndex = i * (this->segments + 1);
		for (unsigned int j = 0; j < this->segments; j++)
		{
			unsigned int index1 = baseIndex + j;
			unsigned int index2 = baseIndex + j + 1;
			indices.push_back(index1);
			indices.push_back(index2);
		}
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndecies*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void VectorFieldRenderer::Draw(Camera &camera, Shader &shaderProgram)
{
	shaderProgram.use();
	glm::mat4 VP = camera.getViewProjectionMatrix();
	glm::mat4 MVP = VP * glm::mat4(1.0f);

	shaderProgram.setUniform4fv("modelViewProjectionMatrix", &MVP[0][0]);

	glBindVertexArray(VAO);
	glDrawElements(GL_LINES, numIndecies, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}