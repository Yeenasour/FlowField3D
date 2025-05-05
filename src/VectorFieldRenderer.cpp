#include <VectorFieldRenderer.h>
#include <VectorField.h>
#include <Engine/Camera.h>
#include <Engine/Shader.h>
#include <Engine/ShaderUtils.h>
#include <Engine/Buffer.h>
#include <vector>


VectorFieldRenderer::VectorFieldRenderer(VectorField &field, int density, float range, int segments) 
	: vf(field), vectorDensity(density), drawingRange(range), segments(segments), numIndecies(2*(segments)*pow(density, 3))
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
	int totalVerts = vecNum*2*(segments + 1);
	VAO->addVBO((VertexBuffer*)new StaticVertexBuffer(nullptr, totalVerts*sizeof(glm::vec3)));
	VAO->setAttribPointer(0, 3, ShaderDataTypeToOpenGLBaseType(ShaderDataType::Float), false, 2 * sizeof(glm::vec3), 0);
	VAO->enableAttribPointer(0);
	VAO->setAttribPointer(1, 3, ShaderDataTypeToOpenGLBaseType(ShaderDataType::Float), false, 2 * sizeof(glm::vec3), (void*)sizeof(glm::vec3));
	VAO->enableAttribPointer(1);

	VAO->setEBO(new IndexBuffer(nullptr, numIndecies*sizeof(unsigned int)));
	VAO->unbind();
	updateBuffers();
}

void VectorFieldRenderer::updateBuffers()
{
	VAO->bind();
	int vecNum = pow(vectorDensity, 3);
	int totalVerts = vecNum*2*(this->segments + 1);
	std::vector<glm::vec3> basePoints;
	basePoints.reserve(vecNum);
	float halfRange = drawingRange / 2;
	for (int i = 0; i < vectorDensity; i++)
	{
		for (int j = 0; j < vectorDensity; j++)
		{
			for (int k = 0; k < vectorDensity; k++)
			{
				float x = -halfRange + i * (drawingRange / (vectorDensity - 1));
				float y = -halfRange + j * (drawingRange / (vectorDensity - 1));
				float z = -halfRange + k * (drawingRange / (vectorDensity - 1));
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
			glm::vec3 color = glm::vec3((1.0 - ((float)j / this->segments)));
			vertecies.push_back(color);
			vert = vert + 0.1f * (vf.evalAt(vert.x, vert.y, vert.z));
		}
	}
	
	VAO->getVBO(0)->subData(vertecies.data(), totalVerts*sizeof(glm::vec3));

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
	
	VAO->getEBO()->subData(indices.data(), numIndecies*sizeof(unsigned int));
	VAO->unbind();
}