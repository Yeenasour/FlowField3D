#version 430 core

precision highp float;

in vec2 fragOffset;
in vec3 color;

out vec4 screenColor;

void main()
{
	float d = length(fragOffset);
    if (d > 1.0) {
        discard;
    }

    screenColor = vec4(color, 1.0);
}
