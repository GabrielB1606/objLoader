//STANDARD
#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<fstream>
#include <string>

//GLEW
#include<GL\glew.h>

//GLFW
#include<GLFW\glfw3.h>

//OPENGL MATH GLM
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>

using namespace std;

//GLOBALS
const GLint WIDTH = 1280, HEIGHT = 720;
GLfloat currentFrame = 0.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// STRUCT VERTEX
struct Vertex{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 textcoord;
};

// TRIANGLE
Vertex vertices[] = {
    glm::vec3(-0.5f, 0.5f, 0.f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f, 1.f),
    glm::vec3(-0.5f, -0.5f, 0.f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 0.f),
    glm::vec3(0.5f, -0.5f, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f),

    glm::vec3(0.5f, 0.5f, 0.f), glm::vec3(1.f, 1.f, 0.f), glm::vec2(0.f, 0.f)
};
unsigned nrOfVertices = sizeof(vertices)/sizeof(Vertex);
GLuint indices[] = {
    0, 1, 2,
    0, 2, 3
};
unsigned nrOfIndices = sizeof(indices)/sizeof(GLuint);

//FUNCTIONS
void INIT_GLFW()
{
	glfwInit();
}

void INIT_GLEW()
{
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		throw "Error in GLEW INIT!";
	}
}

GLFWwindow* INIT_WINDOW(const int WIDTH, const int HEIGHT, int &screenW, int &screenH)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "01 TRIANGLE", nullptr, nullptr);

	if (window == nullptr)
	{
		glfwTerminate();
		throw("Error in creating window!");
	}

	glfwGetFramebufferSize(window, &screenW, &screenH);

	glfwMakeContextCurrent(window);

	// glViewport(0, 0, screenW, screenH);

	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return window;
}

bool loadShaders(GLuint &program){
    char infoLog[512];
    GLint success;

    std::string temp = "";
    std::string src = "";

    std::ifstream in_file;

    // vertex
    in_file.open("../../shaders/vertex_core.glsl");

    if(in_file.is_open()){

        while( std::getline(in_file, temp) ){
            src += temp + "\n";
        }

    }else{
        std::cout << "shader didn't open lol\n";
        return false;
    }

    in_file.close();

    GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
    const GLchar* vertSrc = src.c_str();
    glShaderSource( vertexShader, 1, &vertSrc, NULL );
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if( !success ){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "vertex shader compile error:\n" << infoLog << "\n";
        return false;
    }

    // fragment

    temp = "";
    src = "";

    in_file.open("../../shaders/fragment_core.glsl");

    if(in_file.is_open()){

        while( std::getline(in_file, temp) ){
            src += temp + "\n";
        }

    }else{
        std::cout << "shader didn't open lol\n";
        return false;
    }

    in_file.close();

    GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
    const GLchar* fragSrc = src.c_str();
    glShaderSource( fragmentShader, 1, &fragSrc, NULL );
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if( !success ){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "fragment shader compile error:\n" << infoLog << "\n";
        return false;
    }

    // program
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if( !success ){
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "program link error:\n" << infoLog << "\n";
        return false;
    }

    // end
    glUseProgram(0);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;

}

void updateInput(GLFWwindow* window){

    if( glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS )
        glfwSetWindowShouldClose(window, GLFW_TRUE);

}

int main()
{
	int screenW = 0, screenH = 0;

	//INIT GLFW
	INIT_GLFW();

	//INIT GLFW WINDOW
	GLFWwindow *window = INIT_WINDOW(WIDTH, HEIGHT, screenW, screenH);

	//INIT CONTEXT FOR OPENGL
	INIT_GLEW();

    // OPENGL OPTIONS
    glEnable(GL_DEPTH_TEST);

    	//BACKFACE CULLING AND CC
	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise

        // BLENDING COLORS
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // GL_LINE for outlines

    // SHADER INIT
    GLuint core_program;
    loadShaders(core_program);

    // VAO
    GLuint VAO;
    glCreateVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // could be dynamic draw

    // EBO
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // VERTEXATTRIBUTEPOINTERS AND ENABLE (INPUT ASSEMBLY)
        // POSITION
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

        // COLOR
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

        // TEXTCOORD
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, textcoord));
    glEnableVertexAttribArray(2);

    // BIND VAO 0
    glBindVertexArray(0);

    // matrix stuff
    glm::mat4 ModelMatrix(1.f);
    ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.f));
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f));
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(0.f, 1.f, 0.f));
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(0.f, 0.f, 1.f));
    ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.f));

    glUseProgram(core_program);
    
    glUniformMatrix4fv(glGetUniformLocation(core_program, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));

    glUseProgram(0);

	 //MAIN LOOP
	while (!glfwWindowShouldClose(window))
	{
		currentFrame = (GLfloat)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();

		//Update
        updateInput(window);

		//Clear window
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // Draw
        glUseProgram(core_program);

        // update uniforms
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians(deltaTime * 15.f), glm::vec3(0.f, 0.f, 1.f));
        glUniformMatrix4fv(glGetUniformLocation(core_program, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);
        // glDrawArrays(GL_TRIANGLES, 0, 6);

		//Swap buffers
		glfwSwapBuffers(window);
        glFlush();

        glBindVertexArray(0);
        glUseProgram(0);
	}

	//TERMINATE GLFW
    glfwDestroyWindow(window);
	glfwTerminate();
    glDeleteProgram(core_program);

	return 0;
}