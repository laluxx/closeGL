#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include "stdbool.h"

// For Shaders
#include <stdio.h>
#include "stdlib.h"
#include "string.h"


void Clear();

char* readShaderSource(const char* shaderFilePath);
GLuint compileShader(GLenum type, const char* source);
GLuint linkProgram(GLuint vertexShader, GLuint fragmentShader);

GLuint initializeShaders();
void deleteShaderPrograms(GLuint shaderProgram);

const int screenWidth  = 1920;
const int screenHeight = 1080;

int main(void) {
    GLFWwindow* window;

    // Initialize the library
    if (!glfwInit()) return -1;

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(640, 480, "Glfw", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));


    // Initialize GLEW
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        glfwTerminate();
        return -1;
    }


    float vertices[] = {
        // Positions         // Colors
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // Red
        0.0f,  0.5f,  0.0f,  0.0f, 0.0f, 1.0f, // Green
        0.5f,  -0.5f, 0.0f,  0.0f, 1.0f, 0.0f  // Blue
    };

    // Vertex Buffer Object, Vertex Array Object
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind the VAO and VBO, and upload the vertex data to the buffer.
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);



    // Specify how OpenGL should interpret the vertex data.
    /* glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); */
    /* glEnableVertexAttribArray(0); */

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);




    GLuint shaderProgram = initializeShaders();
    if (!shaderProgram) {
        fprintf(stderr, "Failed to initialize shaders.\n");
        return -1;
    }


    while (!glfwWindowShouldClose(window))
    {
        // Render here
        Clear();


        // Use the shader program
        glUseProgram(shaderProgram);

        // Bind the VAO
        glBindVertexArray(VAO);

        // Draw the triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Unbind the VAO
        glBindVertexArray(0);


        // Swap front and back buffers
        glfwSwapBuffers(window);
        // Poll for and process events
        glfwPollEvents();
    }

    deleteShaderPrograms(shaderProgram);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}





GLuint initializeShaders() {
    char* vertexShaderSource = readShaderSource("./shaders/simple.vert");
    char* fragmentShaderSource = readShaderSource("./shaders/simple.frag");

    if (!vertexShaderSource || !fragmentShaderSource) {
        return 0; // Return 0 to indicate failure
    }

    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    free(vertexShaderSource);
    free(fragmentShaderSource);

    if (!vertexShader || !fragmentShader) {
        return 0; // Return 0 to indicate failure
    }

    GLuint shaderProgram = linkProgram(vertexShader, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram; // Return the shader program ID
}

void deleteShaderPrograms(GLuint shaderProgram) {
    glDeleteProgram(shaderProgram);
}





// SHADER Read Compile and Link
char* readShaderSource(const char* shaderFilePath) {
    FILE* file = fopen(shaderFilePath, "rb");
    if (!file) {
        fprintf(stderr, "Failed to open %s\n", shaderFilePath);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* content = (char*)malloc(length + 1);
    if (!content) {
        fprintf(stderr, "Failed to allocate memory for shader content\n");
        fclose(file);
        return NULL;
    }

    fread(content, 1, length, file);
    content[length] = '\0';
    fclose(file);

    return content;
}

GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    // Error checking
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::COMPILATION_FAILED\n%s\n", infoLog);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint linkProgram(GLuint vertexShader, GLuint fragmentShader) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Error checking
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
        glDeleteProgram(program);
        return 0;
    }

    return program;
}




bool clearOnlyOnce = false;
bool colorCleared = 0;
float background[4] = {0.043f, 0.043f, 0.043f, 1.0f};

/* void Clear() { */
/*     if (clearOnlyOnce && !colorCleared) { */
/*         glClearColor(background[0], background[1], background[2], background[3]); */
/*         printf("cleared\n"); */
/*         colorCleared = true; */
/*     } else if (!clearOnlyOnce) { */
/*         glClearColor(background[0], background[1], background[2], background[3]); */
/*         printf("cleared\n"); */
/*     } */

/*     glClear(GL_COLOR_BUFFER_BIT); */
/* } */

void Clear() {
    static int clearCount = 0; // Static variable to count clears
    int colorChangeFrequency = 16; // Number of clears before changing color

    if (clearOnlyOnce && !colorCleared) {
        glClearColor(background[0], background[1], background[2], background[3]);
        printf("\033[1;31mcleared\033[0m\n"); // Red color for first clear
        colorCleared = true;
        clearCount++;
    } else if (!clearOnlyOnce) {
        glClearColor(background[0], background[1], background[2], background[3]);
        // Change text color based on clearCount
        printf("\033[1;%dmcleared %d\033[0m\n", 31 + ((clearCount / colorChangeFrequency) % 6), clearCount);
        clearCount++;
    }

    glClear(GL_COLOR_BUFFER_BIT);
}
