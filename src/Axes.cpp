#include <Axes.h>
#include <Engine/Camera.h>
#include <Engine/Shader.h>
#include <Engine/ShaderUtils.h>
#include <Engine/Buffer.h>
#include <glm/gtc/matrix_transform.hpp>


Axes::Axes(float len)
	: length(len), Renderable()
{
	initAxes();
	updateModelMatrix();
}

Axes::~Axes()
{
	
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
	
	VAO->bind();
	VAO->addVBO((VertexBuffer*)new StaticVertexBuffer(vertices, sizeof(vertices)));
	VAO->setAttribPointer(0, 3, ShaderDataTypeToOpenGLBaseType(ShaderDataType::Float), false, 6 * sizeof(float), 0);
	VAO->enableAttribPointer(0);
	VAO->setAttribPointer(1, 3, ShaderDataTypeToOpenGLBaseType(ShaderDataType::Float), false, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO->enableAttribPointer(1);
	VAO->setEBO(new IndexBuffer(indicies, sizeof(indicies)));
	VAO->unbind();
}

void Axes::updateModelMatrix()
{
	glm::mat4 mat = glm::mat4(1.0f);
	modelMatrix = glm::scale(mat, glm::vec3(length));
}

void Axes::setLength(float len)
{
	this->length = len;
}