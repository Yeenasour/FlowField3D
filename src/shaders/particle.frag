#version 430 core

precision highp float;

in vec2 fragOffset;
in float speed;

out vec4 screenColor;

void main()
{
	float d = length(fragOffset);
    if (d > 1.0) {
        discard;
    }

	//float v = length(velocity);

	float t = clamp(speed / 4.0, 0.0, 1.0);

    vec3 color = mix(vec3(1.0), vec3(0.957,0.251,0.227), t);

    screenColor = vec4(color, 1.0);
}
