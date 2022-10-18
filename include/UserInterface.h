#pragma once

class UserInterface{
private:
    
    GLFWwindow* window;

    const char* glsl_version;

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;

    // Game State
    bool
        *menuClicked,
        *backFaceCullingOn,
        *antialiasingOn,
        *zBufferOn,
        *fillOn,
        *verticesOn,
        *boundingBoxOn,
        *edgesOn,
        *normalsOn,
        *clearScenePressed;

    const glm::vec4 *clear_color, *normals_color;
    int* indexModelSelected;

    ImGuiStyle style;

    bool SameLine(){ ImGui::SameLine(); return false; }

public:
    UserInterface(GLFWwindow* window, const char* glsl_version, bool* menuClicked, glm::vec4* clear_color, glm::vec4* normals_color, bool* backFaceCullingOn, bool* antialiasingOn, bool* zBufferOn, bool* fillOn, bool* verticesOn, bool* boundingBoxOn, bool* edgesOn, bool* normalsOn, bool* clearScenePressed, int* indexModelSelected);
    ~UserInterface();

    void update(Model* modelSelected, char* modelNames[], size_t nrOfModels );
    void render();
};

void UserInterface::update( Model* modelSelected, char* modelNames[], size_t nrOfModels ){
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
        
        if (ImGui::BeginMainMenuBar()){
            if (ImGui::BeginMenu("GL Options")){
                
                if (ImGui::MenuItem("Back Face Culling", NULL, *backFaceCullingOn)){
                    *(this->backFaceCullingOn) = !*(this->backFaceCullingOn);
                    *menuClicked = true;
                }

                if (ImGui::MenuItem("Antialiasing", NULL, *antialiasingOn)){
                    *antialiasingOn = !*antialiasingOn;
                    *menuClicked = true;
                }

                if (ImGui::MenuItem("Z Buffer", NULL, *zBufferOn)){
                    *zBufferOn = !*zBufferOn;
                    *menuClicked = true;
                }
                
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Import/Export")){
                
                if (ImGui::MenuItem("Import OBJ", NULL)){
                    *backFaceCullingOn = !*backFaceCullingOn;
                    *menuClicked = true;
                }

                if (ImGui::MenuItem("Import MTL", NULL)){
                    *antialiasingOn = !*antialiasingOn;
                    *menuClicked = true;
                }

                if (ImGui::MenuItem("Import Scene", NULL)){
                    *zBufferOn = !*zBufferOn;
                    *menuClicked = true;
                }

                if (ImGui::MenuItem("Export Scene", NULL)){
                    *zBufferOn = !*zBufferOn;
                    *menuClicked = true;
                }
                
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        // SHOW
        ImGui::Separator();
        ImGui::Text("Show");               // Display some text (you can use a format strings too)

        *menuClicked |= 
            ImGui::Checkbox("Fill", fillOn) ||
            SameLine() ||
            ImGui::Checkbox("Vertices", verticesOn) ||
            SameLine() ||
            ImGui::Checkbox("Edges", edgesOn) ||
            ImGui::Checkbox("Bounding Box", boundingBoxOn) ||
            SameLine() ||
            ImGui::Checkbox("Normals", normalsOn);
        
        if(*verticesOn && modelSelected!=nullptr)
            ImGui::SliderFloat("vertex size", modelSelected->getVertexSizeReference(), 1.0f, 20.0f);
        
        // COLORS
        ImGui::Separator();
        ImGui::Text("Color");               // Display some text (you can use a format strings too)

        if( *normalsOn )
            ImGui::ColorEdit3("Normals", (float*)normals_color); // Edit 3 floats representing a color

        ImGui::ColorEdit3("Clear", (float*)clear_color); // Edit 3 floats representing a color
        if( modelSelected != nullptr ){
            if(*fillOn)
                ImGui::ColorEdit3("Fill", (float*)modelSelected->getMaterialReference()->getFillColorReference()); // Edit 3 floats representing a color
            
            if(*verticesOn){
                ImGui::ColorEdit3("Vertex", (float*)modelSelected->getMaterialReference()->getVertexColorReference()); // Edit 3 floats representing a color
                
            }

            if(*edgesOn)
                ImGui::ColorEdit3("Edge", (float*)modelSelected->getMaterialReference()->getEdgeColorReference()); // Edit 3 floats representing a color
        }

        // SCENE
        ImGui::Separator();
        ImGui::Text("Scene");               // Display some text (you can use a format strings too)
        
        if (ImGui::Button("Clear Scene")){
            *clearScenePressed = true;
            *menuClicked = true;
        }

        if (ImGui::Button("Select Camera")){
            *indexModelSelected = -1;
        }

        ImGui::ListBox("Models", indexModelSelected, modelNames, nrOfModels, 4);

         // DEMO LEFTOVERS
        ImGui::Separator();
        ImGui::Text("Demo");               // Display some text (you can use a format strings too)
        
        // ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);


        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

        

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        
        ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }

}

void UserInterface::render(){
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

}

UserInterface::UserInterface(GLFWwindow* window, const char* glsl_version, bool* menuClicked, glm::vec4* clear_color, glm::vec4* normals_color, bool* backFaceCullingOn, bool* antialiasingOn, bool* zBufferOn, bool* fillOn, bool* verticesOn, bool* boundingBoxOn, bool* edgesOn, bool* normalsOn, bool* clearScenePressed, int* indexModelSelected):
    menuClicked(menuClicked),
    clear_color(clear_color),
    normals_color(normals_color),
    backFaceCullingOn(backFaceCullingOn),
    antialiasingOn(antialiasingOn),
    zBufferOn(zBufferOn),
    fillOn(fillOn),
    verticesOn(verticesOn),
    boundingBoxOn(boundingBoxOn),
    edgesOn(edgesOn),
    normalsOn(normalsOn),
    clearScenePressed(clearScenePressed),
    indexModelSelected(indexModelSelected)
{
    this->window = window;
    this->glsl_version = glsl_version;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    this->style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(this->window, true);
    ImGui_ImplOpenGL3_Init( glsl_version );

}

UserInterface::~UserInterface(){

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

}
