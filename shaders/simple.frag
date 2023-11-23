#version 330 core
out vec4 FragColor;

in vec3 color; // receive color from vertex shader

void main() {
    FragColor = vec4(color, 1.0); // set the fragment color
}
