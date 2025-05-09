#include <VectorFieldRenderer.h>
#include <VectorField.h>
#include <Engine/Camera.h>
#include <Engine/Shader.h>
#include <Engine/ShaderUtils.h>
#include <Engine/Buffer.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

// TODO Idea: Draw vectors as billboards, strech them in the direction of field, use opacity on top of color to communicate strength

VectorFieldRenderer::VectorFieldRenderer(VectorField &field, int density, float range) 
	: vf(field), vectorDensity(density), drawingRange(range), Renderable()
{
	initBuffers();
}

VectorFieldRenderer::~VectorFieldRenderer()
{
	
}

void VectorFieldRenderer::initBuffers()
{
	VAO->bind();

	int vecNum = pow(vectorDensity, 3);
	VAO->addVBO((VertexBuffer*)new StaticVertexBuffer(nullptr, 2*vecNum*sizeof(glm::vec3)));
	VAO->setAttribPointer(0, 3, ShaderDataTypeToOpenGLBaseType(ShaderDataType::Float), false, 2 * sizeof(glm::vec3), 0);
	VAO->enableAttribPointer(0);
	VAO->setAttribDivisor(0, 1);
	VAO->setAttribPointer(1, 3, ShaderDataTypeToOpenGLBaseType(ShaderDataType::Float), false, 2 * sizeof(glm::vec3), (void*)sizeof(glm::vec3));
	VAO->enableAttribPointer(1);
	VAO->setAttribDivisor(1, 1);
	
	const float vertices[] = {
		0.4f, 0.1f, 0.0f,
		0.0f, 1.0f, 0.0f,
		-0.4f, 0.1f, 0.0f,
		-0.1f, 0.1f, 0.0f,
		-0.1f, -1.0f, 0.0f,
		0.1f, -1.0f, 0.0f,
		0.1f, 0.1f, 0.0f,
	};

	VAO->addVBO((VertexBuffer*)new StaticVertexBuffer(vertices, sizeof(vertices)));
	VAO->setAttribPointer(2, 3, ShaderDataTypeToOpenGLBaseType(ShaderDataType::Float), false, 3 * sizeof(const float), (void*)0);
	VAO->enableAttribPointer(2);

	unsigned int indices[] = {
		0, 1, 2,
		3, 4, 5,
		3, 5, 6
	};

	VAO->setEBO(new IndexBuffer(indices, sizeof(indices)));
	VAO->unbind();
	updateBuffers();
}

void VectorFieldRenderer::updateBuffers()
{
	VAO->bind();
	int vecNum = pow(vectorDensity, 3);
	std::vector<glm::vec3> vertices;
	vertices.reserve(2*vecNum);
	for (int i = 0; i < vectorDensity; i++)
	{
		for (int j = 0; j < vectorDensity; j++)
		{
			for (int k = 0; k < vectorDensity; k++)
			{
				float t = 2 * drawingRange / (vectorDensity - 1);
				float x = -drawingRange + i * t;
				float y = -drawingRange + j * t;
				float z = -drawingRange + k * t;
				glm::vec3 start(x, y, z);
				glm::vec3 force(vf.evalAt(x, y, z));
				vertices.push_back(start);
				vertices.push_back(force);
			}
		}
	}

	VAO->getVBO(0)->bind();
	VAO->getVBO(0)->subData(vertices.data(), 2*vecNum*sizeof(glm::vec3));
	VAO->unbind();
}