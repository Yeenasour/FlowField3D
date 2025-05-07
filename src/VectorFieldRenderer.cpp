#include <VectorFieldRenderer.h>
#include <VectorField.h>
#include <Engine/Camera.h>
#include <Engine/Shader.h>
#include <Engine/ShaderUtils.h>
#include <Engine/Buffer.h>
#include <vector>

#include <iostream>
#include <vector>
#include <glm/vec3.hpp>

void printVec3Data(const std::vector<glm::vec3>& vec) {
    for (const auto& v : vec) {
        std::cout << "x: " << v.x << ", y: " << v.y << ", z: " << v.z << std::endl;
    }
}

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
	int numIndices = vecNum * 2;
	VAO->addVBO((VertexBuffer*)new StaticVertexBuffer(nullptr, numIndices*2*sizeof(glm::vec3)));
	VAO->setAttribPointer(0, 3, ShaderDataTypeToOpenGLBaseType(ShaderDataType::Float), false, 2 * sizeof(glm::vec3), 0);
	VAO->enableAttribPointer(0);
	VAO->setAttribPointer(1, 3, ShaderDataTypeToOpenGLBaseType(ShaderDataType::Float), false, 2 * sizeof(glm::vec3), (void*)sizeof(glm::vec3));
	VAO->enableAttribPointer(1);

	std::vector<unsigned int> indices;
	indices.reserve(numIndices);
	for (unsigned int i = 0; i < numIndices; i++)
	{
		indices.push_back(i);
	}

	VAO->setEBO(new IndexBuffer(indices.data(), numIndices*sizeof(unsigned int)));
	VAO->unbind();
	updateBuffers();
}

void VectorFieldRenderer::updateBuffers()
{
	VAO->bind();
	int vecNum = pow(vectorDensity, 3);
	int totalVerts = vecNum*4;
	std::vector<glm::vec3> vertices;
	vertices.reserve(vecNum);
	glm::vec3 color1(0.286,1.,0.467);
	glm::vec3 color2(1.,0.337,0.337);
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
				float len = glm::length(force);
				glm::vec3 color = glm::mix(color1, color2, len / 10.0f);

				vertices.push_back(start);
				vertices.push_back(color);
				vertices.push_back(start + 0.5f * (force / len));
				vertices.emplace_back(0);
			}
		}
	}

	VAO->getVBO(0)->bind();
	VAO->getVBO(0)->subData(vertices.data(), totalVerts*sizeof(glm::vec3));
	VAO->unbind();
}