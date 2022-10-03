#include "includes.h"

//GLOBALS
const GLint WIDTH = 1280, HEIGHT = 720;
GLfloat currentFrame = 0.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
const GLfloat movementSpeed = 0.3f;


// TRIANGLE
Vertex vertices[] = {
    glm::vec3(-0.5f, 0.5f, 0.f),    glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f, 1.f), glm::vec3(0.f, 0.f, 1.f),
    glm::vec3(-0.5f, -0.5f, 0.f),   glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 0.f), glm::vec3(0.f, 0.f, 1.f),
    glm::vec3(0.5f, -0.5f, 0.f),    glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f), glm::vec3(0.f, 0.f, 1.f),
    glm::vec3(0.5f, 0.5f, 0.f),     glm::vec3(1.f, 1.f, 0.f), glm::vec2(0.f, 0.f), glm::vec3(0.f, 0.f, 1.f)
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

void updateInput(GLFWwindow* window, Mesh* objectSelected, glm::vec3& lightPos){

    if( glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS )
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    
    if( glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS )
        objectSelected->move( glm::vec3( 0.f, 0.f,  -movementSpeed * deltaTime ) );
    
    if( glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS )
        objectSelected->move( glm::vec3( 0.f, 0.f,  movementSpeed * deltaTime ) );
    
    if( glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS )
        objectSelected->move( glm::vec3( -movementSpeed * deltaTime, 0.f, 0.f ) );
    
    if( glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS )
        objectSelected->move( glm::vec3( movementSpeed * deltaTime, 0.f, 0.f ) );

    if( glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS )
        objectSelected->rotate( glm::vec3(0.f, 100 * movementSpeed * deltaTime, 0.f) );

    if( glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS )
        objectSelected->rotate( glm::vec3(0.f, -100 * movementSpeed * deltaTime, 0.f) );
    
    if( glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS )
        objectSelected->scaleUp( glm::vec3( -movementSpeed * deltaTime ) );
    
    if( glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS )
        objectSelected->scaleUp( glm::vec3( movementSpeed * deltaTime ) );

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
    Shader core_program("../../shaders/vertex_core.glsl", "../../shaders/fragment_core.glsl");

    Mesh test(vertices, nrOfVertices, indices, nrOfIndices);

    Material material0(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f));

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
    core_program.setMat4fv(ViewMatrix, "ViewMatrix");
    core_program.setMat4fv(ProjectionMatrix, "ProjectionMatrix");

    core_program.setVec3f(lightPos0, "lightPos0");
    core_program.setVec3f(camPosition, "camPosition");

	 //MAIN LOOP
	while (!glfwWindowShouldClose(window))
	{
		currentFrame = (GLfloat)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();

		//Update
        updateInput(window, &test, lightPos0);

		//Clear window
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // Draw

        // update uniforms
        core_program.setVec3f(lightPos0, "lightPos0");
    

        ProjectionMatrix = glm::mat4(1.f);
        glfwGetFramebufferSize(window, &bufferScreenW, &bufferScreenH);
        ProjectionMatrix = glm::perspective( glm::radians(fov), static_cast<float>(bufferScreenW)/bufferScreenH, nearPlane, farPlane );

        core_program.setMat4fv(ProjectionMatrix, "ProjectionMatrix");

        material0.sendToShader(core_program);   

        test.render(&core_program);

        // glDrawArrays(GL_TRIANGLES, 0, 6);

		//Swap buffers
		glfwSwapBuffers(window);
        glFlush();

        glBindVertexArray(0);
        core_program.stopUsing();
	}

	//TERMINATE GLFW
    glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}