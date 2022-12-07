#pragma once

void LoadMTL(std::string mtlFile, std::unordered_map<std::string, Material*> *materialMap){

    // attributes
    std::string name = ""; 
    glm::vec3
        diffuse(0.8f), // defines the diffuse color of the material to be (r,g,b). The default is (0.8,0.8,0.8); 
        ambient(0.2f), // defines the ambient color of the material to be (r,g,b). The default is (0.2,0.2,0.2); 
        specular(1.f); // defines the specular color of the material to be (r,g,b). This color shows up in highlights. The default is (1.0,1.0,1.0); 
    
    GLfloat
        transparency = 1.f, // defines the non-transparency of the material to be alpha. The default is 1.0 (not transparent at all). The quantities d and Tr are the opposites of each other, and specifying transparency or nontransparency is simply a matter of user convenience. 
        shininess = 0.f; // defines the shininess of the material to be s. The default is 0.0; 
    
    GLint illumination = 1; // denotes the illumination model used by the material. illum = 1 indicates a flat material with no specular highlights, so the value of Ks is not used. illum = 2 denotes the presence of specular highlights, and so a specification for Ks is required. 

    Texture
        *map_ka = nullptr,
        *map_kd = nullptr,
        *map_ks = nullptr;

    // file variables
    std::stringstream ss;
    std::ifstream file( mtlFile.c_str() );
    std::string line = "";

    // line parse variables    
    std::string prefix = "";

    if( !file.is_open() ){
        std::cout << "ERROR: could not open MTL: " << mtlFile << "\n";
        return;
    }

    while( std::getline(file, line) ){

        ss.clear();
        ss.str(line);

        ss >> prefix;

        if( prefix == "newmtl" ){

            if(name.length() != 0){
                if( materialMap->count(name) )
                    delete (*materialMap)[name];
                
                materialMap->insert_or_assign(name, new Material(name, ambient, diffuse, specular, map_ka, map_kd, map_ks) );
            }

            // reset to default
            illumination = 1;
            shininess = 0;
            transparency = 1;
            ambient = glm::vec3(0.2f);
            diffuse = glm::vec3(0.8f);
            specular = glm::vec3(1.f);
            map_ka = nullptr;
            map_ks = nullptr;
            map_kd = nullptr;
            ss >> name;

        }else if( prefix == "Ns" ){
            ss >> shininess;
        }else if( prefix == "d" ){
            ss >> transparency;
        }else if( prefix == "Tr" ){
            ss >> transparency;
            transparency = 1 - transparency;
        }else if( prefix == "Tf" ){

        }else if( prefix == "illum" ){
            ss >> illumination;
        }else if( prefix == "Ka" ){ // ambient 
            ss >> ambient.x >> ambient.y >> ambient.z;
        }else if( prefix == "Kd" ){ // diffuse
            ss >> diffuse.x >> diffuse.y >> diffuse.z;
        }else if(prefix == "Ks"){   // specular
            ss >> specular.x >> specular.y >> specular.z;
        }else if(prefix == "map_Ka"){   // ambient mapping
            // std::getline(ss, prefix);
            ss >> prefix;
            map_ka = new Texture(prefix, GL_TEXTURE_2D, GL_TEXTURE0);
        }else if(prefix == "map_Kd"){   // diffuse mapping
            // std::getline(ss, prefix);
            ss >> prefix;
            map_kd = new Texture(prefix, GL_TEXTURE_2D, GL_TEXTURE1);
        }else if(prefix == "map_Ks"){   // specular mapping
            // std::getline(ss, prefix);
            ss >> prefix;
            map_ks = new Texture(prefix, GL_TEXTURE_2D, GL_TEXTURE2);
        }

    }

    file.close();

    if(name.length() != 0){
        if( materialMap->count(name) )
            delete (*materialMap)[name];
        
        materialMap->insert_or_assign(name, new Material(name, ambient, diffuse, specular, map_kd, map_ka, map_ks) );
    }

}

std::vector<Model *> LoadOBJ(const char* objFile, std::unordered_map<std::string, Material*> *materialMap){

    std::vector<Model *> models;
    std::vector<Mesh *> meshes;

    // model attribute values
    std::string name = "";
    std::string groupName = "";
    std::string materialName = "";
    GLenum renderType = GL_TRIANGLES;
    glm::vec3 maxComponents(0.f), minComponents(0.f);

    // vertex data
    float maxVertex = 0.f;
    std::vector<glm::vec3> positionVertex;
    std::vector<glm::vec2> texcoordVertex;
    std::vector<glm::vec3> normalVertex;

    std::vector<glm::vec3> approxNormalVertex;
    std::unordered_map<std::string, size_t> approxNormalIndex;

    // vertex indices
    std::vector<GLuint> positionIndex;
    std::vector<GLuint> texcoordIndex;
    std::vector<GLuint> normalIndex;

    // vector of complete vertices
    std::vector<Vertex> vertexArray;

    // file variables
    std::stringstream ss;
    std::ifstream file(objFile);
    std::string line = "";

    // line parse variables    
    std::string fileMTL = "";
    std::string prefix = "";
    
    std::vector< GLuint* > face;

    std::string tmp_str;
    glm::vec3 tmp_vec3;
    glm::vec2 tmp_vec2;
    GLint tmp_int;
    size_t tmp_uint;


    if( !file.is_open() )
        std::cout << "ERROR: could not open OBJ: " << objFile << "\n";

    while( std::getline(file, line) ){

        ss.clear();
        ss.str(line);

        ss >> prefix;

        if(prefix == "o" || prefix == "g" || prefix == "usemtl"){ // new object name

            if( positionIndex.size() != 0 ){

                if( !groupName.length() )
                    if( !materialName.length() )
                        groupName = materialName;
                    else if( !name.length() )
                        groupName = name + " mesh";
                    
                
                if( materialName != "" && materialMap->find(materialName) != materialMap->end() ){

                    meshes.push_back(
                        new Mesh(
                            groupName,
                            positionVertex,
                            texcoordVertex,
                            normalVertex.size()>0? normalVertex:approxNormalVertex,
                            positionIndex,
                            texcoordIndex,
                            normalIndex,
                            (*materialMap)[materialName]
                        )
                    );
                
                }else{
                    
                    tmp_str = "default"+std::to_string(default_material_counter);
                    materialMap->insert_or_assign(tmp_str, new Material(tmp_str));
                    default_material_counter++;

                    meshes.push_back(
                        new Mesh(
                            groupName,
                            positionVertex,
                            texcoordVertex,
                            normalVertex.size()>0? normalVertex:approxNormalVertex,
                            positionIndex,
                            texcoordIndex,
                            normalIndex,
                            (*materialMap)[tmp_str]
                        )
                    );
                    
                }
                
                // positionVertex.clear();
                // texcoordVertex.clear();
                // normalVertex.clear();
                
                positionIndex.clear();
                texcoordIndex.clear();
                normalIndex.clear();
                groupName = "";

                // vertexArray.clear();
                // renderType = 0;
            }

            if( prefix == "g" ){
                if( ss.peek() == ' ' )
                    ss.ignore(1, ' ');
                std::getline(ss, groupName);
            }else if(prefix == "usemtl"){   //  use material
            
                if( ss.peek() == ' ' )
                    ss.ignore(1, ' ');
                
                std::getline(ss, materialName);
                // ss >> materialName;

            }else if(prefix == "o"){

                if(meshes.size()>0){

                    if( name.length() == 0 )
                        if( groupName.length() != 0 )
                            name = groupName + " Model";
                        else if( materialName.length() != 0 )
                            name = materialName + " Model";
                        else
                            name = objFile;

                    models.push_back( new Model( name, meshes ) );
                    models.back()->setBoundingBox((*materialMap)["Bounding Box"], maxComponents, minComponents);
                }

                maxComponents = glm::vec3(0.f);
                minComponents = glm::vec3(0.f);
                meshes.clear();
                ss >> name;

            }

        }else if(prefix == "v"){  //  vertex position

            ss >> tmp_vec3.x >> tmp_vec3.y >> tmp_vec3.z;
            
            maxVertex = std::max(
                maxVertex, std::max(
                    std::abs(tmp_vec3.x), std::max(
                        std::abs(tmp_vec3.y), std::abs(tmp_vec3.z) 
                    )
                )
            );

            positionVertex.push_back( tmp_vec3 );

        }else if(prefix == "vt"){   //  texcoord
            
            ss >> tmp_vec2.x >> tmp_vec2.y;
            texcoordVertex.push_back( tmp_vec2 );

        }else if(prefix == "vn"){  //  vertex normal

            ss >> tmp_vec3.x >> tmp_vec3.y >> tmp_vec3.z;
            normalVertex.push_back( tmp_vec3 );

        }else if(prefix == "f"){    //  face
            
            for( GLuint* &index : face )
                delete index;

            face.clear();

            for(size_t i = 0; ss.rdbuf()->in_avail(); i++ ){
                
                ss >> tmp_int;
                if( ss.fail() )
                    break;

                face.push_back( new GLuint[3] );
                face[i][2] = -1;
                face[i][0] = tmp_int;

                maxComponents.x = std::max( maxComponents.x, positionVertex[tmp_int-1].x );
                maxComponents.y = std::max( maxComponents.y, positionVertex[tmp_int-1].y );
                maxComponents.z = std::max( maxComponents.z, positionVertex[tmp_int-1].z );

                minComponents.x = std::min( minComponents.x, positionVertex[tmp_int-1].x );
                minComponents.y = std::min( minComponents.y, positionVertex[tmp_int-1].y );
                minComponents.z = std::min( minComponents.z, positionVertex[tmp_int-1].z );
                
                if( ss.peek() == '/' ){

                    ss.ignore(1, '/');
                    
                    if( texcoordVertex.size()>0 )
                        ss >> face[i][1];
                    
                    if( ss.peek() == '/' ){

                        ss.ignore(1, '/');

                        if( normalVertex.size()>0 ){
                            ss >> face[i][2];
                        }else if( ss.peek() != ' ' )
                            ss >> tmp_int;
                    
                    }

                }

            }

            if( normalVertex.size() == 0 ){
                tmp_vec3 = 
                    glm::cross(
                        (positionVertex[face[1][0]-1] - positionVertex[face[0][0]-1])
                        ,
                        (positionVertex[face[2][0]-1] - positionVertex[face[0][0]-1])
                    );
                tmp_vec3 = glm::normalize(tmp_vec3);
                tmp_str = (std::to_string( tmp_vec3.x ) +","+std::to_string( tmp_vec3.y ) +","+std::to_string( tmp_vec3.z ));
                
                if( approxNormalIndex.find(tmp_str) == approxNormalIndex.end() ){
                    approxNormalVertex.push_back( tmp_vec3 );
                    tmp_uint = approxNormalVertex.size();
                    approxNormalIndex.insert_or_assign(tmp_str, tmp_uint);
                }else{
                    tmp_uint = approxNormalIndex[tmp_str];
                }
            }

            switch (face.size()){
                case 3:

                    for(size_t i=0; i<face.size(); i++){
                        positionIndex.push_back( face[i][0] );

                        if( texcoordVertex.size()>0 )
                            texcoordIndex.push_back(face[i][1]);
                            
                        if( normalVertex.size()>0 )
                            normalIndex.push_back(face[i][2]);
                        else
                            normalIndex.push_back(tmp_uint);
                    }
                    
                    break;
            
                case 4:

                    for(int i : {0, 1, 2, 0, 2, 3}){
                        positionIndex.push_back( face[i][0] );

                        if( texcoordVertex.size()>0 )
                            texcoordIndex.push_back(face[i][1]);
                        
                        if( normalVertex.size()>0 )
                            normalIndex.push_back(face[i][2]);
                        else
                            normalIndex.push_back(tmp_uint);
                    }
                    break;
            }

        }else if( prefix == "mtllib" ){

            if( ss.peek() == ' ' )
                ss.ignore(1, ' ');

            std::getline(ss, fileMTL);
            LoadMTL( "../../obj/"+fileMTL, materialMap );
        }

    }

    for( GLuint* &index : face )
        delete index;

    face.clear();

    file.close();

    if(groupName == "" )
        if(materialName != "")
            groupName = materialName;
        else if(name != "")
            groupName = name + " mesh";

    if( materialName != "" && materialMap->find(materialName) != materialMap->end() ){
        meshes.push_back(
            new Mesh(
                groupName,
                positionVertex,
                texcoordVertex,
                normalVertex.size()>0? normalVertex:approxNormalVertex,
                positionIndex,
                texcoordIndex,
                normalIndex,
                (*materialMap)[materialName]
            )
        );
    }else{
        
        tmp_str = "default"+std::to_string( default_material_counter );
        materialMap->insert_or_assign(tmp_str, new Material(tmp_str));
        default_material_counter++;

        meshes.push_back(
            new Mesh(
                groupName,
                positionVertex,
                texcoordVertex,
                normalVertex.size()>0? normalVertex:approxNormalVertex,
                positionIndex,
                texcoordIndex,
                normalIndex,
                (*materialMap)[tmp_str]
            )
        );
        
    }

    if( name.length() == 0 )
        if( groupName.length() != 0 )
            name = groupName + " Model";
        else if( materialName.length() != 0 )
            name = materialName + " Model";
        else
            name = objFile;

    models.push_back( new Model( name, meshes ) );
    models.back()->setBoundingBox((*materialMap)["Bounding Box"], maxComponents, minComponents);

    for(Model* &m : models)
        m->normalize(maxVertex);

    return models;

}

void LoadSCN( std::string scnFile, std::vector<Model*> &models, std::unordered_map<std::string, Material*> &materialMap){

     // file variables
    std::stringstream ss;
    std::ifstream file( scnFile.c_str() );
    std::string line = "";
    
    std::string file2read = "";
    std::vector<Model*> modelsLoaded;

    glm::vec3 tmp_vec3;

    // line parse variables    
    std::string prefix = "";

    if( !file.is_open() ){
        std::cout << "ERROR: could not open SCN: " << scnFile << "\n";
        return;
    }

    while( std::getline(file, line) ){
        
        ss.clear();
        ss.str(line);

        ss >> prefix;

        if(prefix == "obj"){

            std::getline(ss, file2read);
            modelsLoaded = LoadOBJ(file2read.c_str(), &materialMap);
            
            if(!std::getline(file, line))
                break;

            ss.clear();
            ss.str(line);

            ss >> prefix;

            if(prefix == "pos"){
                
                ss >> tmp_vec3.x >> tmp_vec3.y >> tmp_vec3.z;

                for(Model* &m:modelsLoaded)
                    m->move( tmp_vec3 );

                if(!std::getline(file, line))
                    break;

                ss.clear();
                ss.str(line);

                ss >> prefix;

            }

            if( prefix == "rot" ){
                ss >> tmp_vec3.x >> tmp_vec3.y >> tmp_vec3.z;

                for(Model* &m:modelsLoaded)
                    m->rotate( tmp_vec3 );
            }

            for(Model* &m:modelsLoaded)
                models.push_back(m);

        }else if(prefix == "mtl"){
            std::getline(ss, file2read);
            LoadMTL(file2read, &materialMap);
        }

    }

}