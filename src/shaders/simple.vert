#version 430 core

precision highp float;

layout (location=0) in vec3 vertexPos;
layout (location=1) in vec3 vertexColor;

uniform mat4 modelViewProjectionMatrix;

out vec3 color;

void main() {
	gl_Position = modelViewProjectionMatrix * vec4(vertexPos, 1.0);
	color = vertexColor;
}