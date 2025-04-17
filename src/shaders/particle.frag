#version 430 core

precision highp float;

in float plifetime;
in vec2 fragOffset;

out vec4 screenColor;

void main()
{
    float alpha = clamp(plifetime, 0.0, 1.0);

	float d = length(fragOffset);
    if (d > 1.0) {
        discard;
    }

    vec3 color = mix(vec3(0.0), vec3(1.0), alpha);

    screenColor = vec4(color, alpha);

    if (alpha < 0.01)
        discard;
}
