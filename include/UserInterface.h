#pragma once

class UserInterface{
private:
    
    GLFWwindow* window;

    const char* glsl_version;

    // Tree porperties
    static const ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    bool *state;

    const glm::vec4 *clear_color, *normals_color;
    size_t* indexModelSelected;
    size_t* indexMeshSelected;

    ImGuiStyle style;

    bool SameLine(){ ImGui::SameLine(); return false; }

public:
    UserInterface(GLFWwindow* window, const char* glsl_version, bool* state, glm::vec4* clear_color, glm::vec4* normals_color, size_t* indexModelSelected, size_t* indexMeshSelected);
    ~UserInterface();

    void update( std::vector<Model*> &models, Moveable* &objectSelected);
    void render();

    bool clickOutside();
};

bool UserInterface::clickOutside(){
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    return !io.WantCaptureMouse;
}

void UserInterface::update( std::vector<Model*> &models, Moveable* &objectSelected ){
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    // ImGui::ShowDemoWindow(&show_demo_window);
    ImGui::ShowDemoWindow();

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
        
        if (ImGui::BeginMainMenuBar()){
            if (ImGui::BeginMenu("GL Options")){
                
                if (ImGui::MenuItem("Back Face Culling", NULL, state[CULLING])){
                    state[CULLING] = !state[CULLING];
                    state[MENU_CLICK] = true;
                }

                if (ImGui::MenuItem("Antialiasing", NULL, state[ANTIALIASING] )){
                    state[ANTIALIASING] = !state[ANTIALIASING];
                    state[MENU_CLICK] = true;
                }

                if (ImGui::MenuItem("Z Buffer", NULL, state[Z_BUFFER])){
                    state[Z_BUFFER] = !state[Z_BUFFER];
                    state[MENU_CLICK] = true;
                }

                if (ImGui::MenuItem("Phong Shading", NULL, state[PHONG_SHADING])){
                    state[PHONG_SHADING] = !state[PHONG_SHADING];
                    state[MENU_CLICK] = true;
                }
                
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Import/Export")){
                
                if (ImGui::MenuItem("Import OBJ", NULL)){
                    state[IMPORT_OBJ] = true;
                    state[MENU_CLICK] = true;
                }

                if (ImGui::MenuItem("Import MTL", NULL)){
                    state[IMPORT_MTL] = true;
                    state[MENU_CLICK] = true;
                }

                if (ImGui::MenuItem("Import Scene", NULL)){
                    state[IMPORT_SCENE] = true;
                    state[MENU_CLICK] = true;
                }

                // if (ImGui::MenuItem("Export Scene", NULL)){
                //     state[EXPORT_SCENE] = true;
                //     state[MENU_CLICK] = true;
                // }
                
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        // SHOW
        ImGui::Separator();
        ImGui::Text("Show");               // Display some text (you can use a format strings too)

        state[MENU_CLICK] |= 
            ImGui::Checkbox("Fill", &state[SHOW_FILL]) ||
            SameLine() ||
            ImGui::Checkbox("Vertices", &state[SHOW_VERTICES]) ||
            SameLine() ||
            ImGui::Checkbox("Edges", &state[SHOW_EDGES]) ||
            ImGui::Checkbox("Bounding Box", &state[SHOW_BOUNDING_BOX]) ||
            SameLine() ||
            ImGui::Checkbox("Normals", &state[SHOW_NORMALS]);
        
        if(state[SHOW_VERTICES] && *indexModelSelected != -1)
            ImGui::SliderFloat("vertex size", models[*indexModelSelected]->getVertexSizeReference(), 1.0f, 20.0f);
        
        // COLORS
        ImGui::Separator();
        ImGui::Text("Color");               // Display some text (you can use a format strings too)

        if( state[SHOW_NORMALS] )
            ImGui::ColorEdit3("Normals", (float*)normals_color); // Edit 3 floats representing a color

        ImGui::ColorEdit3("Clear", (float*)clear_color); // Edit 3 floats representing a color
        if( *indexMeshSelected != -1 ){
            if( state[SHOW_FILL] )
                ImGui::ColorEdit3("Fill", (float*)models[*indexModelSelected]->getMeshesReferences()[*indexMeshSelected]->getMaterialReference()->getFillColorReference() ); // Edit 3 floats representing a color
            
            if( state[SHOW_VERTICES] ){
                ImGui::ColorEdit3("Vertex", (float*)models[*indexModelSelected]->getMeshesReferences()[*indexMeshSelected]->getMaterialReference()->getVertexColorReference()); // Edit 3 floats representing a color
                
            }

            if( state[SHOW_EDGES] )
                ImGui::ColorEdit3("Edge", (float*)models[*indexModelSelected]->getMeshesReferences()[*indexMeshSelected]->getMaterialReference()->getEdgeColorReference()); // Edit 3 floats representing a color
        }

        // OBJECT SELECTED
        ImGui::Separator();
        ImGui::Text("Object Selected");
        if( *indexModelSelected != -1 ){
            Moveable *selected;
            
            if( *indexMeshSelected != -1 )
                selected = models[*indexModelSelected]->getMeshesReferences()[*indexMeshSelected];
            else
                selected = models[*indexModelSelected];

            ImGui::Text("Scale ");
            ImGui::SameLine();
            if( ImGui::Button("-") )
                selected->scaleUp( glm::vec3(-0.5f) );
            ImGui::SameLine();
            if( ImGui::Button("+") )
                selected->scaleUp( glm::vec3(0.5f) );

            if( *indexMeshSelected != -1 ){
                ImGui::SliderFloat("Mesh Size X", models[*indexModelSelected]->getMeshesReferences()[*indexMeshSelected]->getSizeXRef() , -30.0f, 30.0f);
                ImGui::SliderFloat("Mesh Size Y", models[*indexModelSelected]->getMeshesReferences()[*indexMeshSelected]->getSizeYRef() , -30.0f, 30.0f);
                ImGui::SliderFloat("Mesh Size Z", models[*indexModelSelected]->getMeshesReferences()[*indexMeshSelected]->getSizeZRef() , -30.0f, 30.0f);
            }else{
                ImGui::SliderFloat("Model Size X", models[*indexModelSelected]->getSizeXRef() , -30.0f, 30.0f);
                ImGui::SliderFloat("Model Size Y", models[*indexModelSelected]->getSizeYRef() , -30.0f, 30.0f);
                ImGui::SliderFloat("Model Size Z", models[*indexModelSelected]->getSizeZRef() , -30.0f, 30.0f);
            }

            if (ImGui::Button("Delete")){
                if( *indexMeshSelected == -1 ){
                    models.erase( models.begin() + *indexModelSelected );
                }else{
                    models[*indexModelSelected]->removeMesh( *indexMeshSelected );
                }
                *indexModelSelected = -1;
                *indexMeshSelected = -1;
                objectSelected = nullptr;
            }
            
        
        }

        // SCENE
        ImGui::Separator();
        ImGui::Text("Scene");               // Display some text (you can use a format strings too)
        
        if (ImGui::Button("Clear Scene")){
            state[CLEAR_CLICK] = true;
            state[MENU_CLICK] = true;
        }

        if (ImGui::Button("Select Camera")){
            *indexModelSelected = -1;
            *indexMeshSelected = -1;
            objectSelected = nullptr;
        }

        ImGui::Text("Models & Meshes");
            for (size_t i = 0; i < models.size(); i++){

                ImGuiTreeNodeFlags node_flags = base_flags;
                
                if ( i == *indexModelSelected )
                    node_flags |= ImGuiTreeNodeFlags_Selected;

                bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, models[i]->getName().c_str() );

                if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()){
                    *indexModelSelected = i;
                    *indexMeshSelected = -1;
                    objectSelected = models[i];
                }
                if (node_open){
                    for(size_t j = 0; j<models[i]->getMeshesReferences().size(); j++){
                        node_flags = base_flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
                        if( j == *indexMeshSelected )
                            node_flags |= ImGuiTreeNodeFlags_Selected;
                        ImGui::TreeNodeEx((void*)(intptr_t)(j), node_flags, models[i]->getMeshesReferences()[j]->getName().c_str() );

                        if (ImGui::IsItemClicked() ){
                            *indexModelSelected = i;
                            *indexMeshSelected = j;
                            objectSelected = models[i]->getMeshesReferences()[j];
                        }
                    }
                        ImGui::TreePop();
                }

            }
        
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

UserInterface::UserInterface(GLFWwindow* window, const char* glsl_version, bool* state, glm::vec4* clear_color, glm::vec4* normals_color, size_t* indexModelSelected, size_t* indexMeshSelected):
    state(state),
    clear_color(clear_color),
    normals_color(normals_color),
    indexModelSelected(indexModelSelected),
    indexMeshSelected(indexMeshSelected)
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
