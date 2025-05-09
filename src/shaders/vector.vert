#version 430 core

precision highp float;

layout (location=0) in vec3 pos;
layout (location=1) in vec3 dir;
layout (location=2) in vec3 vertexPos;

uniform mat4 modelMatrix;
uniform mat4 viewProjection;
uniform vec3 cameraPos;
uniform float vectorScale;

out vec3 color;

void main() {

	vec3 normDir = normalize(dir);
	vec3 viewDir = normalize(pos - cameraPos);
	vec3 right = -normalize(cross(normDir, viewDir));
	vec3 front = normalize(cross(normDir, right));

	mat3 orient = mat3(
		right,
		normDir,
		front
	);

	vec3 worldPos = pos + (orient * vectorScale * vertexPos);

	gl_Position = viewProjection * modelMatrix * vec4(worldPos, 1.0);
	color = vec3(1.0);
}