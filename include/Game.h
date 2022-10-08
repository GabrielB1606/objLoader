#pragma once

class Game{
private:

    const int SHADER_CORE_PROGRAM = 0;

    // test
    PrimitiveQuad testPrimitive;

    // Framerate
    GLfloat currentFrame = 0.0f;
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;

    // Movement
    const GLfloat movementSpeed = 0.3f;

    // Window
    GLFWwindow* window;
    const int WIDTH, HEIGHT;
    int fbWidth, fbHeight;

    // OpenGL Context
    const int GL_MAJOR, GL_MINOR;

    // Matrices
    glm::mat4 ViewMatrix;
    glm::mat4 ProjectionMatrix;

    // Vectors
    glm::vec3 camPosition;
    glm::vec3 worldUp;
    glm::vec3 camFront;

    // Camera options
    float fov;
    float nearPlane;
    float farPlane;

    // Shaders
    std::vector<Shader*> shaders;

    // Meshes
    std::vector<Mesh*> meshes;

    // Materials
    std::vector<Material*> materials;

    // Lights
    std::vector<glm::vec3*> lights;

    // callback functions
    static void framebuffer_resize(GLFWwindow* window, int fbW, int fbH);

    // init functions
    void initWindow(const char* title, bool resizable);
    void initGLFW();
    void initGLEW();
    void initOpenGLOptions();
    void initMatrices();
    void initShaders();
    void initMeshes();
    void initMaterials();
    void initLights();
    void initUniforms();

    void updateUniforms();

public:

    Game(const char* title, const int WIDTH, const int HEIGHT, int GLmajor, int GLminor, bool resizable);
    virtual ~Game();

    int getWindowShouldClose();
    void closeWindow();

    void update();
    void updateInput(Mesh* objectSelected);
    void render();
};

void Game::updateUniforms(){
    // update uniforms
    this->materials[0]->sendToShader( *this->shaders[SHADER_CORE_PROGRAM] );

    // update framebuffer size
    glfwGetFramebufferSize( this->window, &this->fbWidth, &this->fbHeight );

    // update ViewMatrix
    this->ViewMatrix=glm::lookAt(this->camPosition, this->camPosition+this->camFront, this->worldUp);
    this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ViewMatrix, "ViewMatrix");

    // update ProjectionMatrix
    ProjectionMatrix = glm::perspective( glm::radians(this->fov), static_cast<float>(this->fbWidth)/this->fbHeight, this->nearPlane, this->farPlane );
    this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");
}

void Game::updateInput(Mesh* objectSelected = nullptr){

    if( glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS )
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    if(objectSelected == nullptr){

        if( glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ){
            this->camPosition.z -= movementSpeed*deltaTime;
        }
        if( glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ){
            this->camPosition.z += movementSpeed*deltaTime;
        }
        if( glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ){
            this->camPosition.x -= movementSpeed*deltaTime;
        }
        if( glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ){
            this->camPosition.x += movementSpeed*deltaTime;
        }
        if( glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS ){
            this->camPosition.y -= movementSpeed*deltaTime;
        }
        if( glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS ){
            this->camPosition.y += movementSpeed*deltaTime;
        }

    }else{
        
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

    }
    

    // if( glfwGetKey(window, GLFW_KEY_UP) )
    //     lightPos.z += movementSpeed * 2 *deltaTime;
    // if( glfwGetKey(window, GLFW_KEY_DOWN) )
    //     lightPos.z -= movementSpeed * 2 *deltaTime;
    // if( glfwGetKey(window, GLFW_KEY_LEFT) )
    //     lightPos.x -= movementSpeed * 2 *deltaTime;
    // if( glfwGetKey(window, GLFW_KEY_RIGHT) )
    //     lightPos.x += movementSpeed * 2 *deltaTime;

}

void Game::initUniforms(){
    this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ViewMatrix, "ViewMatrix");
    this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");

    this->shaders[SHADER_CORE_PROGRAM]->setVec3f(*this->lights[0], "lightPos0");
    this->shaders[SHADER_CORE_PROGRAM]->setVec3f(this->camPosition, "camPosition");
}

void Game::initLights(){
    this->lights.push_back( new glm::vec3(0.f, 0.f, 2.f) );
}

void Game::initMeshes(){
    // this->meshes.push_back( new Mesh( testPrimitive ) );
    this->meshes.push_back( new Mesh( PrimitivePyramid() ) );
}

void Game::initMaterials(){
    materials.push_back( new Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f)) );
}

void Game::initShaders(){
    shaders.push_back( new Shader(GL_MAJOR, GL_MINOR, "../../shaders/vertex_core.glsl", "../../shaders/fragment_core.glsl") );
}

void Game::initMatrices(){

        // View
    this->ViewMatrix = glm::mat4(1.f);
    this->ViewMatrix = glm::lookAt(this->camPosition, this->camPosition + this->camFront, this->worldUp);

        // Projection
    this->ProjectionMatrix = glm::mat4(1.f);
    ProjectionMatrix = glm::perspective( glm::radians(this->fov), static_cast<float>(this->fbWidth)/this->fbHeight, this->nearPlane, this->farPlane );

}

void Game::closeWindow(){
    glfwSetWindowShouldClose(this->window, GLFW_TRUE);
}

int Game::getWindowShouldClose(){
    return glfwWindowShouldClose(this->window);
}

void Game::render(){

    //Update
    updateInput( );

    //Clear window
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    updateUniforms();

    // draw
    for( Mesh* m : meshes )
        m->render( this->shaders[SHADER_CORE_PROGRAM] );

    // End Draw - Swap buffers
    glfwSwapBuffers(window);
    glFlush();

    // Reset
    glBindVertexArray(0);
    glUseProgram(0);
}

void Game::update(){

    this->currentFrame = (GLfloat)glfwGetTime();
    this->deltaTime = this->currentFrame - this->lastFrame;
    this->lastFrame = this->currentFrame;

    // Update input
    glfwPollEvents();


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

    this->fov = 90.f;
    this->nearPlane = 0.1f;
    this->farPlane = 1000.f;

    this->worldUp = glm::vec3(0.f, 1.f, 0.f);
    this->camPosition = glm::vec3(0.f, 0.f, 1.f);
    this->camFront = glm::vec3(0.f, 0.f, -1.f);
    
    this->window = nullptr;
    this->fbHeight = this->HEIGHT;
    this->fbWidth = this->WIDTH;

    this->initGLFW();
    this->initWindow(title, resizable);
    this->initGLEW();
    this->initOpenGLOptions();
    this->initMatrices();
    this->initShaders();
    this->initMaterials();
    this->initMeshes();
    this->initLights();
    this->initUniforms();

}

Game::~Game(){
    for(size_t i = 0; i<this->lights.size(); i++)
        delete this->lights[i];

    for(size_t i = 0; i<this->shaders.size(); i++)
        delete this->shaders[i];

    for(size_t i = 0; i<this->materials.size(); i++)
        delete this->materials[i];

    for(size_t i = 0; i<this->meshes.size(); i++)
        delete this->meshes[i];

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
}