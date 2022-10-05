#pragma once

class Game{
private:
    // window
    GLFWwindow* window;
    const int WIDTH, HEIGHT;
    int fbWidth, fbHeight;

    // OpenGL Context
    const int GL_MAJOR, GL_MINOR;

    static void framebuffer_resize(GLFWwindow* window, int fbW, int fbH);

    void initWindow(const char* title, bool resizable);
    void initGLFW();
    void initGLEW();
    void initOpenGLOptions();


public:
    Game(const char* title, const int WIDTH, const int HEIGHT, int GLmajor, int GLminor, bool resizable);
    virtual ~Game();

    int getWindowShouldClose();
    void closeWindow();

    void update();
    void render();
};

void Game::closeWindow(){
    glfwSetWindowShouldClose(this->window, GLFW_TRUE);
}

int Game::getWindowShouldClose(){
    return glfwWindowShouldClose(this->window);
}

void Game::update(){

}

void Game::initOpenGLOptions(){
    glEnable(GL_DEPTH_TEST);

    	//BACKFACE CULLING AND CC
	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise

        // BLENDING COLORS
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // GL_LINE for outlines
}

void Game::initGLEW(){
    glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		std::cout << "Error in GLEW INIT!\n";
	}
}

void Game::initGLFW(){
    if( glfwInit() == GLFW_FALSE ){
        std::cout << "Error glfwInit\n";
        glfwTerminate();
    }
}

Game::Game(const char* title, const int windowWIDTH, const int windowHEIGHT, int GLmajor, int GLminor, bool resizable):WIDTH(windowWIDTH), HEIGHT(windowHEIGHT), GL_MAJOR(GLmajor), GL_MINOR(GLminor){

    this->window = nullptr;
    this->fbHeight = this->HEIGHT;
    this->fbWidth = this->WIDTH;

    this->initGLFW();
    this->initWindow(title, resizable);
    this->initGLEW();
    this->initOpenGLOptions();

}

Game::~Game(){
    glfwDestroyWindow(this->window);
    glfwTerminate();
}

void Game::framebuffer_resize(GLFWwindow* window, int fbW, int fbH){
    glViewport(0, 0, fbW, fbH);
}

void Game::initWindow(const char* title, bool resizable){
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->GL_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->GL_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	this->window = glfwCreateWindow(this->WIDTH, this->HEIGHT, title, nullptr, nullptr);

	if (this->window == nullptr)
	{
		glfwTerminate();
		throw("Error in creating window!");
	}

	glfwGetFramebufferSize(this->window, &this->fbWidth, &this->fbHeight);
    glfwSetFramebufferSizeCallback(this->window, Game::framebuffer_resize);

	glfwMakeContextCurrent(this->window);

	// glViewport(0, 0, screenW, screenH);

	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}