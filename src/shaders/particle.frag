#version 430 core

precision highp float;

in float plifetime;
out vec4 screenColor;

void main()
{
    float alpha = clamp(plifetime, 0.0, 1.0);

    vec3 color = mix(vec3(0.0), vec3(1.0), alpha);

    screenColor = vec4(color, alpha);

    if (alpha < 0.01)
        discard;
}
