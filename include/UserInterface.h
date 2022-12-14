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

    size_t indexLightSelected = -1;

    ImGuiStyle style;

    bool SameLine(){ ImGui::SameLine(); return false; }

public:
    UserInterface(GLFWwindow* window, const char* glsl_version, bool* state, glm::vec4* clear_color, glm::vec4* normals_color, size_t* indexModelSelected, size_t* indexMeshSelected);
    ~UserInterface();

    void update( std::vector<Model*> &models, Moveable* &objectSelected, std::vector<Light*> &lights);
    void render();

    bool clickOutside();
};

bool UserInterface::clickOutside(){
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    return !io.WantCaptureMouse;
}

void UserInterface::update( std::vector<Model*> &models, Moveable* &objectSelected, std::vector<Light*> &lights ){
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    ImGui::ShowDemoWindow(&show_demo_window);

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

        // SHADERS
        ImGui::Separator();
        ImGui::Text("Lighting");               // Display some text (you can use a format strings too)

        state[MENU_CLICK] |= 
            ImGui::Checkbox("Ambient", &state[AMBIENT]) ||
            SameLine() ||
            ImGui::Checkbox("Diffuse", &state[DIFFUSE]) ||
            SameLine() ||
            ImGui::Checkbox("Specular", &state[SPECULAR]);
        
        ImGui::Text("Shader");
        const char* itemsShaderCombo[] = { "None", "Flat", "Gouraud", "Phong" };
        static int currentShader = 0;
        ImGui::Combo("Shader Type", &currentShader, itemsShaderCombo, IM_ARRAYSIZE(itemsShaderCombo));

        switch (currentShader){
            case 0:
                state[FLAT] = false;
                state[PHONG] = false;
                state[GOURAUD] = false;
                break;
            case 1:
                state[FLAT] = true;
                state[GOURAUD] = false;
                state[PHONG] = false;
                break;
            case 2:
                state[FLAT] = false;
                state[GOURAUD] = true;
                state[PHONG] = false;
                break;
            case 3:
                state[FLAT] = false;
                state[GOURAUD] = false;
                state[PHONG] = true;
                break;
            
            default:
                break;
        }

        ImGui::Text("Lights");

            if( ImGui::Button("Add Point Light") ){
                if( lights.size() < 3 )
                    lights.push_back( new PointLight( glm::vec3(0.f, 0.f, 2.f) ) );
            }
            ImGui::SameLine();
            if( ImGui::Button("Add Directional Light") ){
                if( lights.size() < 3 )
                    lights.push_back( new DirectionalLight( glm::vec3(0.f, 0.f, 2.f) ) );
            }

            for (size_t i = 0; i < lights.size(); i++){

                ImGuiTreeNodeFlags node_flags = base_flags;
                
                if ( i == indexLightSelected )
                    node_flags |= ImGuiTreeNodeFlags_Selected;

                std::string lightLabel =  "Light " + std::to_string(i) + " (" + lightNames[ lights[i]->getType() ] + ")";

                bool node_open_light = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, lightLabel.c_str() );

                if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()){
                    indexLightSelected = i;
                }

                if (node_open_light){
                    ImGui::DragFloat("Intensity", (lights[i])->getIntensityReference() , 0.15f);
                    ImGui::ColorEdit3("Light Color", (float*)(lights[i])->getColorReference()); // Edit 3 floats 
                    
                    switch ( lights[i]->getType() )
                    {
                        case POINT_LIGHT:
                            ImGui::DragFloat3("Light Position", (float*)((PointLight*)lights[i])->getPositionReference(), 0.1f);

                            ImGui::Text("Attenuation Coefficients"); 
                            ImGui::DragFloat("Constant", ((PointLight*)lights[i])->getConstantReference() , 0.015f);
                            ImGui::DragFloat("Linear", ((PointLight*)lights[i])->getLinearReference() , 0.015f);
                            ImGui::DragFloat("Quadratic", ((PointLight*)lights[i])->getQuadraticReference() , 0.015f);
                            break;
                        
                        case DIR_LIGHT:
                            ImGui::DragFloat3("Light Direction", (float*)((DirectionalLight*)lights[i])->getDirectionReference(), 0.1f);
                            break;
                        
                        default:
                            break;
                    }
                    if( ImGui::Button("Remove Light") ){
                        lights.erase( lights.begin()+i );
                    }
                    ImGui::TreePop();
                }

            }

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

            if( *indexMeshSelected != -1 ){
                if(  state[SPECULAR] ){
                    ImGui::DragFloat( "Shininess", (float*)models[*indexModelSelected]->getMeshesReferences()[*indexMeshSelected]->getMaterialReference()->getShininessReference() );
                }
                ImGui::Checkbox( "Use Texture", models[*indexModelSelected]->getMeshesReferences()[*indexMeshSelected]->getUseTextureReference() );
                ImGui::Text("Load Texture Maps");
                if( ImGui::Button("Ambient Map") ){
                    char const * lFilterPatterns[3] = { "*.jpg", "*.jpeg", "*.png" };
                    char* filePath = tinyfd_openFileDialog(
                        "Import Texture File",
                        "",
                        3,
                        lFilterPatterns,
                        NULL,
                        0
                    );
                    
                    if( filePath == NULL )
                        return;
                    
                    models[*indexModelSelected]->getMeshesReferences()[*indexMeshSelected]->getMaterialReference()->assignTexture(AMBIENT_MAP, std::string(filePath) );

                }
                SameLine();
                if( ImGui::Button("Diffuse Map") ){
                    char const * lFilterPatterns[3] = { "*.jpg", "*.jpeg", "*.png" };
                    char* filePath = tinyfd_openFileDialog(
                        "Import Texture File",
                        "",
                        3,
                        lFilterPatterns,
                        NULL,
                        0
                    );
                    
                    if( filePath == NULL )
                        return;
                    
                    models[*indexModelSelected]->getMeshesReferences()[*indexMeshSelected]->getMaterialReference()->assignTexture(DIFFUSE_MAP, std::string(filePath) );

                }
                SameLine();
                if( ImGui::Button("Specular Map") ){
                    char const * lFilterPatterns[3] = { "*.jpg", "*.jpeg", "*.png" };
                    char* filePath = tinyfd_openFileDialog(
                        "Import Texture File",
                        "",
                        3,
                        lFilterPatterns,
                        NULL,
                        0
                    );
                    
                    if( filePath == NULL )
                        return;
                    
                    models[*indexModelSelected]->getMeshesReferences()[*indexMeshSelected]->getMaterialReference()->assignTexture(SPECULAR_MAP, std::string(filePath) );

                }
                
                const char* itemsTexGenCombo[] = { "None", "Plane", "Spherical"};
                ImGui::Combo("Texture Coordinates Generation", models[*indexModelSelected]->getMeshesReferences()[*indexMeshSelected]->getTexCoordGenReference(), itemsTexGenCombo, IM_ARRAYSIZE(itemsTexGenCombo));

            }


            ImGui::Text("Scale ");
            ImGui::SameLine();
            if( ImGui::Button("-") )
                selected->scaleUp( glm::vec3(-0.5f) );
            ImGui::SameLine();
            if( ImGui::Button("+") )
                selected->scaleUp( glm::vec3(0.5f) );

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

                bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)(i+3), node_flags, models[i]->getName().c_str() );

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
