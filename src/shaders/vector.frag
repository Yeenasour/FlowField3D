#version 430 core

precision highp float;

in vec4 color;

out vec4 screenColor;

void main() {
	screenColor = color;
}