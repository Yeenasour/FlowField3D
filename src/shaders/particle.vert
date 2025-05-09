#version 430 core

precision highp float;

layout (location=0) in vec2 quad;
layout (location=1) in vec3 instancePos;
layout (location=2) in float lifetime;

uniform float particleSize;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec2 fragOffset;
out vec3 color;

void main() {
	vec3 right = vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
	vec3 up = vec3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);

	vec3 pos = instancePos + (lifetime / 2) * particleSize * (right * quad.x + up * quad.y);
	gl_Position = projectionMatrix * viewMatrix * vec4(pos, 1.0);

	color = vec3(1.0);
	fragOffset = quad;
}