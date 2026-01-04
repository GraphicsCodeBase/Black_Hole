#version 430 core

// Input from vertex shader
in vec3 worldPos;
in float distFromBlackHole;

// Output color
out vec4 FragColor;

// Uniforms
uniform float u_Rs;

void main() {
    // Base grid color: semi-transparent white/cyan
    vec3 gridColor = vec3(0.3, 0.8, 1.0);  // Cyan-ish

    // Fade based on distance from black hole
    // Brighter near black hole, dimmer far away
    float brightness = 1.0;
    if (distFromBlackHole > u_Rs) {
        brightness = clamp(1.0 - (distFromBlackHole / (u_Rs * 10.0)), 0.2, 1.0);
    }

    // Make it semi-transparent so we can see through it
    float alpha = 0.4 * brightness;

    FragColor = vec4(gridColor * brightness, alpha);
}
