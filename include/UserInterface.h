#pragma once

class UserInterface{
private:
    
    GLFWwindow* window;

    const char* glsl_version;

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;

    ImGuiStyle style;

public:
    UserInterface(GLFWwindow* window, const char* version_glsl);
    ~UserInterface();

    void update(bool* menuClicked, glm::vec4* clear_color, bool* backFaceCullingOn, bool* antialiasingOn, bool* zBufferOn, bool* wireframeOn, bool* verticesOn, bool* boundingBoxOn, bool* edgesOn, bool* clearScenePressed, Model* modelSelected );
    void render();
};

void UserInterface::update(bool* menuClicked, glm::vec4* clear_color, bool* backFaceCullingOn, bool* antialiasingOn, bool* zBufferOn, bool* wireframeOn, bool* verticesOn, bool* boundingBoxOn, bool* edgesOn, bool* clearScenePressed, Model* modelSelected ){
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

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        *menuClicked = 
            ImGui::Checkbox("Back Face Culling", backFaceCullingOn) ||
            ImGui::Checkbox("Antialiasing", antialiasingOn) ||
            ImGui::Checkbox("Z Buffer", zBufferOn) ||
            
            ImGui::Checkbox("Wireframe", wireframeOn) ||

            ImGui::Checkbox("Show Vertices", verticesOn) ||
            ImGui::Checkbox("Show Edges", edgesOn) ||
            ImGui::Checkbox("Show Bounding Box", boundingBoxOn);
        
        if (ImGui::Button("Clear Scene")){
            *clearScenePressed = true;
            *menuClicked = true;
        }

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)clear_color); // Edit 3 floats representing a color
        if( modelSelected != nullptr ){
            if(!*wireframeOn)
                ImGui::ColorEdit3("fill color", (float*)modelSelected->getMaterialReference()->getFillColorReference()); // Edit 3 floats representing a color
            
            if(*verticesOn){
                ImGui::ColorEdit3("vertex color", (float*)modelSelected->getMaterialReference()->getVertexColorReference()); // Edit 3 floats representing a color
                ImGui::SliderFloat("vertex size", modelSelected->getVertexSizeReference(), 1.0f, 20.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            }

            if(*edgesOn)
                ImGui::ColorEdit3("edge color", (float*)modelSelected->getMaterialReference()->getEdgeColorReference()); // Edit 3 floats representing a color
        }

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
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

UserInterface::UserInterface(GLFWwindow* window, const char* glsl_version){
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
