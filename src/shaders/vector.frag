#version 430 core

precision highp float;


in vec3 color;

out vec4 screenColor;


void main() {
	screenColor = vec4(color, 1.0);
}