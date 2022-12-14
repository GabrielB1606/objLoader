#pragma once

class Game{
private:

    enum SHADER_PROGRAM{
        SHADER_CORE_PROGRAM = 0,
        SHADER_NORMALS_PROGRAM,
        SHADER_PICKING_PROGRAM,
        SHADER_PHONG_PROGRAM
        
    };

    // Framerate
    GLfloat currentFrame;
    GLfloat deltaTime;
    GLfloat lastFrame;

    // Movement Speed
    GLfloat movementSpeed = 0.75f;

    // Mouse Movement
    double lastMouseX, lastMouseY;
    double mouseX, mouseY;
    double mouseOffsetX, mouseOffsetY;
    bool firstMouse;
    int mouseButtonPressState = GLFW_RELEASE;

    // Window
    GLFWwindow* window;
    const int WIDTH, HEIGHT;
    int fbWidth, fbHeight;

    // scene variables & stuff
    SHADER_PROGRAM coreProgramIndex = SHADER_CORE_PROGRAM;
    Moveable* objectSelected = nullptr;
    size_t modelSelected = -1, meshSelected = -1;
    glm::vec4 clearColor;
    glm::vec4 normalsColor;
    bool guiState[21] = {
        false,  // menu clicked   
        false,  // clear scene button clicked
        true,   // culling toggle
        true,   // antialiasing toggle
        true,   // zbuffer toggle
        true,   // show fill toggle
        false,  // show vertices toggle
        true,   // show edges toggle
        false,  // show bounding box toggle
        false,  // show normals per vertex toggle
        false,  // import OBJ clicked
        false,  // import MTL clicked
        false,  // import Scene clicked
        false,  // export Scene clicked
        false,  // phong shading
        true,  // ambient
        true,  // diffuse
        true,  // specular
        false,  // flat
        false,  // gouraud
        false   // phong
    };

    // User Interface
    UserInterface* gui;

    // OpenGL Context
    const int GL_MAJOR, GL_MINOR;
    const char* glsl_version;

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

    // Camera
    Camera camera;

    // Shaders
    std::vector<Shader*> shaders;

    // Materials
    // std::vector<Material*> materials;
    std::unordered_map<std::string, Material*> materialMap;

    // Models
    std::vector<Model*> models;

    // Lights
    std::vector<Light*> lights;
    bool lightsOn = false;

    // callback functions
    static void framebuffer_resize(GLFWwindow* window, int fbW, int fbH);

    // init functions
    void initWindow(const char* title, bool resizable);
    void initGLFW();
    void initGLEW();
    void initOpenGLOptions();
    void initUserInterface();

    void initMatrices();
    void initShaders();

    void initMaterials();
    void initModels();
    void initTextures();

    void initLights();
    void initPointLights();
    void initUniforms();

    // update functions
    void updateUniforms();
    void updateDeltaTime();
    void updateInputKeyboard();
    void updateInputMouse();
    void updateInput();
    void updateState();
    void updateMatrices();
    

public:

    Game(const char* title, const int WIDTH, const int HEIGHT, const char* glsl_version, int GLmajor, int GLminor, bool resizable);
    virtual ~Game();

// bool backFaceCullingOn, antialiasingOn, zBufferOn, fillOn, verticesOn, boundingBoxOn, edgesOn;
    void setBackFaceCulling(bool state);
    void setAntialiasing(bool state);
    void setZBuffer(bool state);
    void setFillOn(bool state);
    void setVerticesVisible(bool state);
    void setBoundingBox(bool state);

    void clearScene();

    int getWindowShouldClose();
    void closeWindow();

    void update();
    void render();
    void picking();
};

void Game::initTextures(){

}

void Game::picking(){

    // http://www.opengl-tutorial.org/miscellaneous/clicking-on-objects/picking-with-an-opengl-hack/

    updateUniforms();

    //Clear window
    // glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    this->shaders[SHADER_PICKING_PROGRAM]->setMat4fv(this->ViewMatrix, "ViewMatrix");
    this->shaders[SHADER_PICKING_PROGRAM]->setVec3f(this->camera.getPosition(), "camPosition");
    this->shaders[SHADER_PICKING_PROGRAM]->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");

    for( size_t i = 0; i<models.size(); i++ ){

        // Convert "i", the integer mesh ID, into an RGB color
        size_t r = (i & 0x000000FF) >>  0;
        size_t g = (i & 0x0000FF00) >>  8;
        size_t b = (i & 0x00FF0000) >> 16;

        this->shaders[SHADER_PICKING_PROGRAM]->setVec4f(glm::vec4(r/255.0f, g/255.0f, b/255.0f, 1.0f), "PickingColor");
        models[i]->render( this->shaders[SHADER_PICKING_PROGRAM] );
    }

    // Wait until all the pending drawing commands are really done.
    // Ultra-mega-over slow ! 
    // There are usually a long time between glDrawElements() and
    // all the fragments completely rasterized.
    glFlush();
    glFinish(); 

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Read the pixel at the center of the screen.
    // You can also use glfwGetMousePos().
    // Ultra-mega-over slow too, even for 1 pixel, 
    // because the framebuffer is on the GPU.
    unsigned char data[4];
    glfwGetCursorPos( this->window, &this->mouseX, &this->mouseY );
    glReadPixels( (GLint)(this->mouseX) , (GLint)(this->fbHeight-this->mouseY) ,1,1, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glfwSwapBuffers(window);

    // Convert the color back to an integer ID
    size_t pickedID = 
        data[0] + 
        data[1] * 256 +
        data[2] * 256*256;


    if( pickedID < models.size() ){
        modelSelected = pickedID;
        objectSelected = models[pickedID];
    }else{
        objectSelected = nullptr;
        modelSelected = -1;
        meshSelected = -1;
    }


    // End Draw - Swap buffers

    // Reset
    glBindVertexArray(0);
    glUseProgram(0);

}

void Game::clearScene(){

    modelSelected = -1;

    for(Model* &m : models)
        delete m;
    
    models.clear();

}

void Game::setZBuffer(bool state){

    if(state)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);

}

void Game::setBackFaceCulling(bool state){

    if(state)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);

}

void Game::setAntialiasing(bool state){

    if(state)
        glEnable(GL_MULTISAMPLE);
    else
        glDisable(GL_MULTISAMPLE);

}

void Game::initUserInterface(){
    gui = new UserInterface(this->window, "#version 330", guiState, &clearColor, &normalsColor, &modelSelected, &meshSelected );
}

void Game::initModels(){

    // std::vector<Model*> modelsLoaded = LoadOBJ("../../obj/Crate1.obj", &this->materialMap);
    // this->models.push_back( modelsLoaded[0] );

}

void Game::updateMatrices(){
    // update framebuffer size
    glfwGetFramebufferSize( this->window, &this->fbWidth, &this->fbHeight );

    // update ViewMatrix and Camera Position
    this->ViewMatrix = this->camera.getViewMatrix(); 

    // update ProjectionMatrix
    ProjectionMatrix = glm::perspective( glm::radians(this->fov), static_cast<float>(this->fbWidth)/this->fbHeight, this->nearPlane, this->farPlane );
}

void Game::updateUniforms(){
    updateMatrices(); 
    
    this->shaders[this->coreProgramIndex]->set1i( guiState[FLAT]? 1:0, "flatShading" );
    this->shaders[this->coreProgramIndex]->set1i( guiState[GOURAUD]? 1:0, "gouraudShading" );
    this->shaders[this->coreProgramIndex]->set1i( guiState[PHONG]? 1:0, "phongShading" );
    
    this->shaders[this->coreProgramIndex]->set1i( guiState[AMBIENT]? 1:0, "ambientLighting" );
    this->shaders[this->coreProgramIndex]->set1i( guiState[DIFFUSE]? 1:0, "diffuseLighting" );
    this->shaders[this->coreProgramIndex]->set1i( guiState[SPECULAR]? 1:0, "specularLighting" );

    this->shaders[this->coreProgramIndex]->setMat4fv(this->ViewMatrix, "ViewMatrix");
    this->shaders[this->coreProgramIndex]->setVec3f(this->camera.getPosition(), "camPosition");

    if( guiState[SHOW_NORMALS] ){
        this->shaders[SHADER_NORMALS_PROGRAM]->setMat4fv(this->ViewMatrix, "ViewMatrix");
        this->shaders[SHADER_NORMALS_PROGRAM]->setVec3f(this->camera.getPosition(), "camPosition");
    }

    this->shaders[this->coreProgramIndex]->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");
    if(guiState[SHOW_NORMALS])
        this->shaders[SHADER_NORMALS_PROGRAM]->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");
    
    // update light
    for(size_t i = 0; i<lights.size(); i++ ){
        switch ( lights[i]->getType()){
            case POINT_LIGHT:
                ((PointLight*)lights[i])->sendToShader( *this->shaders[SHADER_CORE_PROGRAM], i );
                break;
            
            case DIR_LIGHT:
                ((DirectionalLight*)lights[i])->sendToShader( *this->shaders[SHADER_CORE_PROGRAM], i );
                break;
            
            default:
                break;
        }
    }

    this->shaders[SHADER_CORE_PROGRAM]->set1i( lights.size(), "n_lights" );

}

void Game::updateInput(){
     // Update input
    glfwPollEvents();

    updateInputKeyboard();
    updateInputMouse();
    // camera.updateInput(deltaTime, -1, this->mouseOffsetX, this->mouseOffsetY);
}

void Game::updateInputMouse(){

    int mouseRightState = glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_RIGHT);

    if( mouseRightState == GLFW_PRESS ){

        glfwGetCursorPos( this->window, &this->mouseX, &this->mouseY );

        if( firstMouse ){
            this->lastMouseX = this->mouseX;
            this->lastMouseY = this->mouseY;
            firstMouse = false;
        }

        this->mouseOffsetX = this->mouseX - this->lastMouseX;
        this->mouseOffsetY = this->mouseY - this->lastMouseY;

        this->lastMouseX = this->mouseX;
        this->lastMouseY = this->mouseY;

        if( modelSelected != -1 ){
            this->objectSelected->rotate( glm::vec3( this->mouseOffsetY*deltaTime*movementSpeed*1000, this->mouseOffsetX*deltaTime*movementSpeed*1000, 0.f ) );
        }else{
            this->camera.updateCameraAngle(deltaTime, mouseOffsetX, mouseOffsetY);
        }

    }else{
        firstMouse = true;
    }

    int newState = glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_LEFT);
    if (newState == GLFW_RELEASE && this->mouseButtonPressState == GLFW_PRESS && gui->clickOutside() ) {
        picking();
    }
    this->mouseButtonPressState = newState;


    if( glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS ){
        // this->lights[0]->setPosition( this->camera.getPosition() );
    }

}

void Game::updateInputKeyboard(){


    if( glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS )
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    if(objectSelected == nullptr){

        if( glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ){
            this->camera.updatePosition(deltaTime, FORWARD);
        }
        if( glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ){
            this->camera.updatePosition(deltaTime, BACKWARD);
        }
        if( glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ){
            this->camera.updatePosition(deltaTime, LEFT);
        }
        if( glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ){
            this->camera.updatePosition(deltaTime, RIGHT);
        }
        if( glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS ){
            this->camera.updatePosition(deltaTime, DOWNWARD);
        }
        if( glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS ){
            this->camera.updatePosition(deltaTime, UPWARD);
        }

    }else{
        
        if( glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS )
            this->objectSelected->move( glm::vec3( 0.f, 0.f, -movementSpeed * deltaTime ) );
        
        if( glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS )
            this->objectSelected->move( glm::vec3( 0.f, 0.f, movementSpeed * deltaTime ) );
        
        if( glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS )
            this->objectSelected->move( glm::vec3( -movementSpeed * deltaTime, 0.f, 0.f ) );
        
        if( glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS )
            this->objectSelected->move( glm::vec3( movementSpeed * deltaTime, 0.f, 0.f ) );

        if( glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS )
            this->objectSelected->rotate( glm::vec3(0.f, 0.f, -100 * movementSpeed * deltaTime) );

        if( glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS )
            this->objectSelected->rotate( glm::vec3(0.f, 0.f, 100 * movementSpeed * deltaTime) );
        
        if( glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS )
            this->objectSelected->scaleUp( glm::vec3( -movementSpeed * deltaTime ) );
        
        if( glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS )
            this->objectSelected->scaleUp( glm::vec3( movementSpeed * deltaTime ) );

        if( glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS ){
            this->objectSelected->move( glm::vec3( 0.f, -movementSpeed * deltaTime, 0.f ) );
        }
        if( glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS ){
            this->objectSelected->move( glm::vec3( 0.f, movementSpeed * deltaTime, 0.f ) );
        }

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

    // if(lightsOn)
    for( Light* &pl : this->lights )
        pl->sendToShader( *this->shaders[SHADER_CORE_PROGRAM] );
    // this->shaders[SHADER_CORE_PROGRAM]->setVec3f(this->camPosition, "camPosition");
}

void Game::initPointLights(){

    this->lights.push_back( new DirectionalLight( glm::vec3(0.f, 0.f, 2.f) ) );

}

void Game::initLights(){

    initPointLights();
}

void Game::initMaterials(){

    this->materialMap.insert({ "default", new Material("default", glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f)) });
    this->materialMap.insert({ "Bounding Box", new Material("Bounding Box", glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f)) });

}

void Game::initShaders(){
    shaders.push_back( new Shader(glsl_version, GL_MAJOR, GL_MINOR, "./shaders/core/vertex_core.glsl", "./shaders/core/fragment_core.glsl", "./shaders/core/geometry_core.glsl" ) );
    shaders.push_back( new Shader(glsl_version, GL_MAJOR, GL_MINOR, "./shaders/core/vertex_core.glsl", "./shaders/normals/fragment_normals.glsl", "./shaders/normals/geometry_normals.glsl" ) );
    shaders.push_back( new Shader(glsl_version, GL_MAJOR, GL_MINOR, "./shaders/core/vertex_core.glsl", "./shaders/picking/fragment_picking.glsl", "./shaders/core/geometry_core.glsl" ) );
    shaders.push_back( new Shader(glsl_version, GL_MAJOR, GL_MINOR, "./shaders/phong/vertex_phong.glsl", "./shaders/phong/fragment_phong.glsl" ) );
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
    // picking();
    //Clear window
    glClearColor(  clearColor.x, clearColor.y, clearColor.z, clearColor.w );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    updateUniforms();
    // glActiveTexture(GL_TEXTURE0);
    // shaders[SHADER_CORE_PROGRAM]->set1i( 0, "tex0" );
    // glActiveTexture(GL_TEXTURE1);
    // shaders[SHADER_CORE_PROGRAM]->set1i( 1, "tex1" );

    for( Model* &m : models ){
        m->render( this->shaders[this->coreProgramIndex], guiState[SHOW_EDGES], guiState[SHOW_VERTICES], guiState[SHOW_FILL], guiState[SHOW_BOUNDING_BOX], guiState[CULLING] );
        if( guiState[SHOW_NORMALS] ){
            this->shaders[SHADER_NORMALS_PROGRAM]->setVec4f(normalsColor, "normalsColor");
            m->render( this->shaders[SHADER_NORMALS_PROGRAM] );
        }
    }
    
    gui->update( models, objectSelected, lights );
    gui->render();

    updateState();

    // End Draw - Swap buffers
    glfwSwapBuffers(window);
    glFlush();

    // Reset
    glBindVertexArray(0);
    glUseProgram(0);
}

void Game::update(){

    updateDeltaTime();
    // updateInput( this->meshes[0] );
    updateInput(  );

    // if( models.size() >= 1 )
    //     models[0]->rotate( deltaTime*glm::vec3(0.f, 30.f, 0.f) );

}

void Game::updateState(){
    if( guiState[MENU_CLICK] ){

        guiState[MENU_CLICK] = false;
    
        setAntialiasing( guiState[ANTIALIASING] );
        setZBuffer( guiState[Z_BUFFER] );
        setBackFaceCulling( guiState[CULLING] );
        
        if( guiState[CLEAR_CLICK] ){
            guiState[CLEAR_CLICK] = false;
            clearScene();
        }else if( guiState[IMPORT_OBJ] ){
            
            guiState[IMPORT_OBJ]  = false;

            char const * lFilterPatterns[1] = { "*.obj" };
            char* filePath = tinyfd_openFileDialog(
                "Import OBJ File",
                "",
                1,
                lFilterPatterns,
                NULL,
                0
            );
            
            if( filePath == NULL )
                return;

            std::vector<Model*> modelsLoaded = LoadOBJ(filePath, &materialMap);
            for( Model* &m : modelsLoaded )
                this->models.push_back(m);
        
        }else if( guiState[IMPORT_MTL] ){
            guiState[IMPORT_MTL] = false;

            char const * lFilterPatterns[1] = { "*.mtl" };
            char* filePath = tinyfd_openFileDialog(
                "Import MTL File",
                "",
                1,
                lFilterPatterns,
                NULL,
                0
            );
            
            if( filePath == NULL )
                return;
            
            LoadMTL( filePath, &materialMap );

        }else if(guiState[IMPORT_SCENE]){
            guiState[IMPORT_SCENE] = false;

            char const * lFilterPatterns[1] = { "*.scn" };
            char* filePath = tinyfd_openFileDialog(
                "Import SCN File",
                "",
                1,
                lFilterPatterns,
                NULL,
                0
            );
            
            if( filePath == NULL )
                return;
            
            LoadSCN( filePath, models, materialMap );

        }else if(guiState[EXPORT_SCENE]){
            guiState[EXPORT_SCENE] = false;
        }else if( guiState[PHONG_SHADING] && coreProgramIndex != SHADER_PHONG_PROGRAM ){
            coreProgramIndex = SHADER_PHONG_PROGRAM;
            lightsOn = true;
        }else if( !guiState[PHONG_SHADING] && coreProgramIndex != SHADER_CORE_PROGRAM ){
            coreProgramIndex = SHADER_CORE_PROGRAM;
            lightsOn = false;
        }

    }
}

void Game::updateDeltaTime(){
    this->currentFrame = (GLfloat)glfwGetTime();
    this->deltaTime = this->currentFrame - this->lastFrame;
    this->lastFrame = this->currentFrame;
}

void Game::initOpenGLOptions(){
    // verticesOn, boundingBoxOn

        // Z BUFFER
    glEnable(GL_DEPTH_TEST);
    glEnable( GL_POLYGON_OFFSET_FILL );
    glEnable( GL_POLYGON_OFFSET_LINE );

    	//BACKFACE CULLING AND CC
	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise

        // BLENDING COLORS
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // WIREFRAME
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // GL_LINE for outlines

        // ANTIALIASING
    glEnable(GL_MULTISAMPLE); 
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

Game::Game(const char* title, const int windowWIDTH, const int windowHEIGHT, const char* glsl_version, int GLmajor, int GLminor, bool resizable):WIDTH(windowWIDTH), HEIGHT(windowHEIGHT), GL_MAJOR(GLmajor), GL_MINOR(GLminor), camera(glm::vec3(0.f, 1.f, 3.f), glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f)){

    this->modelSelected = -1;
    this->glsl_version = glsl_version;

    normalsColor = glm::vec4(0.8f, 0.3f, 0.02f , 1.f);

    // Camera View
    this->fov = 90.f;
    this->nearPlane = 0.1f;
    this->farPlane = 1000.f;

    // Camera Position
    this->worldUp = glm::vec3(0.f, 1.f, 0.f);
    this->camPosition = glm::vec3(0.f, 0.f, 1.f);
    this->camFront = glm::vec3(0.f, 0.f, -1.f);

    // Window variables
    this->window = nullptr;
    this->fbHeight = this->HEIGHT;
    this->fbWidth = this->WIDTH;
    this->clearColor = glm::vec4(0.7f, 0.7f, 0.7f, 1.f);

    // Mouse input
    this->lastMouseX = 0;
    this->lastMouseY = 0;
    this->mouseX = 0;
    this->mouseY = 0;
    this->mouseOffsetX = 0;
    this->mouseOffsetY = 0;
    this->firstMouse = true;

    // Framerate
    this->deltaTime = 0;
    this->lastFrame = 0;
    this->currentFrame = 0;

    this->initGLFW();
    this->initWindow(title, resizable);
    this->initGLEW();
    this->initOpenGLOptions();
    this->initUserInterface();
    
    this->initMatrices();
    this->initShaders();
    
    this->initMaterials();
    this->initModels();

    this->initLights();
        
    this->initUniforms();

    this->initTextures();

}

Game::~Game(){
    for(size_t i = 0; i<this->lights.size(); i++)
        delete this->lights[i];

    for(size_t i = 0; i<this->shaders.size(); i++)
        delete this->shaders[i];

    // for(size_t i = 0; i<this->materials.size(); i++)
    //     delete this->materials[i];

    for(size_t i = 0; i<this->models.size(); i++)
        delete this->models[i];

    delete gui;

    for(const auto & entry : materialMap)
        delete entry.second;

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
    glfwWindowHint(GLFW_SAMPLES, 4);

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