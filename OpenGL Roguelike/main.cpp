#include <iostream>
#include <glad/glad.h>
#include <glfw3.h>
#include <Shader.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


//hardcoded shaders, will replace with file loading later
const char* vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"    //the position variable has attribute position 0 in the VAO
                                 "layout (location = 1) in vec3 aColor;\n"  //the color variable has attribute position 1
                                 "out vec3 ourColor;\n"                     //specify a color output to the fragment shader
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos, 1.0);\n"
                                 "   ourColor = aColor;"                    //set ourColor to the input color we get from vertex data
                                 "}\0";

const char* fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "in vec3 ourColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(ourColor, 1.0);\n"
                                   "}\n\0";

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    //glad loads all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    //Vertex Shader compilation
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }


    //Fragment shader compilation
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }


    //Shader program: actually passes the shaders to the GPU
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKER_FAILED\n" << infoLog << std::endl;
    }

    //deletes the shaders as they are no longer needed on the CPU
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    //vertices for a 2D rectangle
//    float vertices[] = {
//        0.5f,  0.5f, 0.0f, //top right
//        0.5,  -0.5f, 0.0f, //bottom right
//       -0.5f, -0.5f, 0.0f, //bottom left
//       -0.5f,  0.5f, 0.0f
//    };

    unsigned int indices[] = {
        0, 1, 3,    //first triangle
        1, 2, 3     //second triangle
    };

    //vertices for a triangle
//    float vertices[] = {
//        -0.5f, -0.5f, 0.0f, //bottom left
//         0.5f, -0.5f, 0.0f, //bottom right
//         0.0f,  0.5f, 0.0f  //top
//    };

    //vertices for a triangle with a color for each vertex
    float vertices[] = {
        //positions         //colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   //bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   //bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    //top
    };

    //Vertex Buffer Object: holds all vertex data to send to GPU
    //Vertex Array Object: holds vertex attributes
    //Element Buffer Object: Stores an indice that tells OpenGL what sets of vertices to draw
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    //glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //Static draw is used since we only need to write the vertices to the GPU once and then render them each frame
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
   // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //Tells OpenGL how to read the data from the Vertex Buffer Object
    //position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    
    //Draw in wireframe mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    //render loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        //render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //actually uses the shaders when rendering
        glUseProgram(shaderProgram);

        //Causes the triangle to change color each frame
        
        ////updates the uniform color 
        //float timeValue = glfwGetTime();
        //float greenValue = sin(timeValue) / 2.0f + 0.5f;
        //int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        //
        ////4f at the end of of function name specifies that we want to pass 4 floats to the uniform value found in the line above
        ////We could have also used glUniformfv which would pass a float array
        ////In this case we are passing the location of the uniform variable in the shader code and using our four float values 
        ////as a vector4 that represent RGBA
        //glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}