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
GLfloat movementSpeed = 0.3f;

// STRUCT VERTEX
struct Vertex{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 textcoord;
    glm::vec2 normal;
};

// TRIANGLE
Vertex vertices[] = {
    glm::vec3(-0.5f, 0.5f, 0.f),    glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f, 1.f), glm::vec3(0.f, 0.f, -1.f),
    glm::vec3(-0.5f, -0.5f, 0.f),   glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 0.f), glm::vec3(0.f, 0.f, -1.f),
    glm::vec3(0.5f, -0.5f, 0.f),    glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f), glm::vec3(0.f, 0.f, -1.f),
    glm::vec3(0.5f, 0.5f, 0.f),     glm::vec3(1.f, 1.f, 0.f), glm::vec2(0.f, 0.f), glm::vec3(0.f, 0.f, -1.f)
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

void updateInput(GLFWwindow* window, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale, glm::vec3& lightPos){

    if( glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS )
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    
    if( glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS )
        position.z -= movementSpeed * deltaTime;
    
    if( glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS )
        position.z += movementSpeed * deltaTime;
    
    if( glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS )
        position.x -= movementSpeed * deltaTime;
    
    if( glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS )
        position.x += movementSpeed * deltaTime;

    if( glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS )
        rotation.y += 100 * movementSpeed * deltaTime;

    if( glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS )
        rotation.y -= 100 * movementSpeed * deltaTime;
    
    if( glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS )
        scale -= movementSpeed * deltaTime;
    
    if( glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS )
        scale += movementSpeed * deltaTime;

    if( glfwGetKey(window, GLFW_KEY_UP) )
        lightPos.z += movementSpeed * 2 *deltaTime;
    if( glfwGetKey(window, GLFW_KEY_DOWN) )
        lightPos.z -= movementSpeed * 2 *deltaTime;
    if( glfwGetKey(window, GLFW_KEY_LEFT) )
        lightPos.x -= movementSpeed * 2 *deltaTime;
    if( glfwGetKey(window, GLFW_KEY_RIGHT) )
        lightPos.x += movementSpeed * 2 *deltaTime;

}

int main()
{
	int bufferScreenW = 0, bufferScreenH = 0;

	//INIT GLFW
	INIT_GLFW();

	//INIT GLFW WINDOW
	GLFWwindow *window = INIT_WINDOW(WIDTH, HEIGHT, bufferScreenW, bufferScreenH);

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

        // NORMAL
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(3);

    // BIND VAO 0
    glBindVertexArray(0);

    // object parameters
    glm::vec3 position(0.f);
    glm::vec3 rotation(0.f);
    glm::vec3 scale(1.f);

    // matrix stuff
        // Model
    glm::mat4 ModelMatrix(1.f);
    ModelMatrix = glm::translate(ModelMatrix, position);
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians( rotation.x ), glm::vec3(1.f, 0.f, 0.f));
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians( rotation.y ), glm::vec3(0.f, 1.f, 0.f));
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians( rotation.z ), glm::vec3(0.f, 0.f, 1.f));
    ModelMatrix = glm::scale(ModelMatrix, glm::vec3( scale ));

        // View
    glm::vec3 worldUp(0.f, 1.f, 0.f);
    glm::vec3 camPosition(0.f, 0.f, 1.f);
    glm::vec3 camFront(0.f, 0.f, -1.f);
    glm::mat4 ViewMatrix(1.f);
    ViewMatrix = glm::lookAt(camPosition, camPosition + camFront, worldUp);

        // Projection
    float fov = 90.f;
    float nearPlane = 0.1f;
    float farPlane = 1000.f;
    glm::mat4 ProjectionMatrix(1.f);
    ProjectionMatrix = glm::perspective( glm::radians(fov), static_cast<float>(bufferScreenW)/bufferScreenH, nearPlane, farPlane );

    // lights
    glm::vec3 lightPos0(0.f, 0.f, 2.f);

    // init uniforms
    glUseProgram(core_program);
    
    glUniformMatrix4fv(glGetUniformLocation(core_program, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(core_program, "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(ViewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(core_program, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));

    glUniform3fv( glGetUniformLocation(core_program, "lightPos0"), 1, glm::value_ptr(lightPos0) );
    glUniform3fv( glGetUniformLocation(core_program, "camPosition"), 1, glm::value_ptr(camPosition) );

    glUseProgram(0);

	 //MAIN LOOP
	while (!glfwWindowShouldClose(window))
	{
		currentFrame = (GLfloat)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();

		//Update
        updateInput(window, position, rotation, scale, lightPos0);

		//Clear window
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // Draw
        glUseProgram(core_program);

        // update uniforms
        // position.z -= 0.001f;
        // rotation.y += 90.f * deltaTime;
        // lightPos0.y += 0.5f *deltaTime;
        // lightPos0.x += 0.5f *deltaTime;
        glUniform3fv( glGetUniformLocation(core_program, "lightPos0"), 1, glm::value_ptr(lightPos0) );

        ModelMatrix = glm::mat4(1.f);

        ModelMatrix = glm::translate(ModelMatrix, position);
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians( rotation.x ), glm::vec3(1.f, 0.f, 0.f));
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians( rotation.y ), glm::vec3(0.f, 1.f, 0.f));
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians( rotation.z ), glm::vec3(0.f, 0.f, 1.f));
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3( scale ));

        glUniformMatrix4fv(glGetUniformLocation(core_program, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));


        ProjectionMatrix = glm::mat4(1.f);
        glfwGetFramebufferSize(window, &bufferScreenW, &bufferScreenH);
        ProjectionMatrix = glm::perspective( glm::radians(fov), static_cast<float>(bufferScreenW)/bufferScreenH, nearPlane, farPlane );
        glUniformMatrix4fv(glGetUniformLocation(core_program, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));


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