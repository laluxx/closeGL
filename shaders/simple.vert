#version 330 core
layout (location = 0) in vec3 aPos;   // position variable
layout (location = 1) in vec3 aColor; // color variable

out vec3 color; // pass color to fragment shader

void main() {
    gl_Position = vec4(aPos, 1.0);
    color = aColor; // set ourColor to the input color from the VBO
}
