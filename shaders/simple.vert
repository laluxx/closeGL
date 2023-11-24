// #version 330 core
// layout (location = 0) in vec3 aPos;   // position variable
// layout (location = 1) in vec3 aColor; // color variable

// out vec3 color; // pass color to fragment shader

// void main() {
//     gl_Position = vec4(aPos, 1.0);
//     color = aColor; // set ourColor to the input color from the VBO
// }


#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform float time; // pass time value from cpu

out vec3 color;

void main() {
    vec3 pos = aPos;
    pos.y += sin(pos.x * 10.0 + time) * 0.0;
    pos.x += sin(pos.x * 10.0 + time) * 0.0;
    pos.z += sin(pos.x * 10.0 + time) * 0.0;
    pos.b += sin(pos.x * 10.0 + time) * 0.0;
    pos.g += sin(pos.x * 10.0 + time) * 0.0;
    pos.p += sin(pos.x * 10.0 + time) * 0.0;
    pos.r += sin(pos.x * 10.0 + time) * 0.0;
    pos.s += sin(pos.x * 10.0 + time) * 0.0;
    pos.t += sin(pos.x * 10.0 + time) * 0.0;
    gl_Position = vec4(pos, 1.0);
    color = aColor;
}
