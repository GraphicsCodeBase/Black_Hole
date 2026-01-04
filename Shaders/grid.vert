#version 430 core

// Input: vertex position from mesh
layout(location = 0) in vec3 aPos;

// Uniforms
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform vec2 u_blackHolePos;  // Black hole position in XY
uniform float u_Rs;            // Schwarzschild radius
uniform float u_warpStrength;  // How much to warp the grid

// Output to fragment shader
out vec3 worldPos;
out float distFromBlackHole;

void main() {
    // Transform position to world space
    vec4 worldPosition = u_Model * vec4(aPos, 1.0);
    worldPos = worldPosition.xyz;

    // Calculate distance from black hole (in XZ plane - horizontal plane)
    vec2 toBlackHole = worldPos.xz - u_blackHolePos;
    distFromBlackHole = length(toBlackHole);

    // Apply gravitational warping (creates the "funnel" effect)
    // The closer to the black hole, the more it pulls down (negative Y)
    if (distFromBlackHole > u_Rs) {
        // Warping formula: deeper near black hole, flattens out further away
        float warp = -u_warpStrength * u_Rs / distFromBlackHole;
        worldPosition.y += warp;
    } else {
        // Inside event horizon: pull down maximally
        worldPosition.y -= u_warpStrength * 2.0;
    }

    // Update world position after warping
    worldPos = worldPosition.xyz;

    // Transform to clip space
    gl_Position = u_Projection * u_View * worldPosition;
}
