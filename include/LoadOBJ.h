#pragma once

std::vector<Material *> LoadMTL(){
    std::vector<Material*> materials;

    return materials;
}

std::vector<Model *> LoadOBJ(const char* objFile, std::unordered_map<std::string, Material*> *materialMap){

    std::vector<Model *> models;
    std::vector<Mesh *> meshes;

    // model attribute values
    std::string name;
    std::string materialName;
    GLenum renderType = 0;
    int vertexPerFace = 0;

    // vertex data
    float maxVertex = 0.f;
    std::vector<glm::vec3> positionVertex;
    std::vector<glm::vec2> textcoordVertex;
    std::vector<glm::vec3> normalVertex;

    // vertex indices
    std::vector<GLuint> positionIndex;
    std::vector<GLuint> textcoordIndex;
    std::vector<GLuint> normalIndex;

    // vector of complete vertices
    std::vector<Vertex> vertexArray;

    // file variables
    std::stringstream ss;
    std::ifstream file(objFile);
    std::string line = "";

    // line parse variables    
    std::string prefix = "";
    glm::vec3 tmp_vec3;
    glm::vec2 tmp_vec2;
    GLint tmp_int;


    if( !file.is_open() )
        std::cout << "ERROR: could not open OBJ: " << objFile << "\n";

    while( std::getline(file, line) ){


        ss.clear();
        ss.str(line);

        ss >> prefix;

        if(prefix == "o" || prefix == "g"){ // new object name

            if( positionIndex.size() != 0 ){
                
                meshes.push_back( new Mesh(positionVertex, textcoordVertex, normalVertex, positionIndex, textcoordIndex, normalIndex, renderType ) );
                
                positionVertex.clear();
                textcoordVertex.clear();
                normalVertex.clear();
                
                positionIndex.clear();
                textcoordIndex.clear();
                normalIndex.clear();

                vertexArray.clear();
                renderType = 0;
                vertexPerFace = 0;
            }

            if(prefix == "o"){

                if(meshes.size()>0){
                    if( materialMap->find(materialName) != materialMap->end() )
                        models.push_back( new Model( name, meshes, (*materialMap)[materialName] ) );
                    else
                        models.push_back( new Model( name, meshes, (*materialMap)["default"] ) );
                }

                meshes.clear();
                ss >> name;

            }

        }else if(prefix == "v"){  //  vertex position

            ss >> tmp_vec3.x >> tmp_vec3.y >> tmp_vec3.z;
            maxVertex = std::max( maxVertex, std::max(tmp_vec3.x, std::max(tmp_vec3.y, tmp_vec3.z)) );
            positionVertex.push_back( tmp_vec3 );

        }else if(prefix == "vt"){   //  textcoord
            
            ss >> tmp_vec2.x >> tmp_vec2.y;
            textcoordVertex.push_back( tmp_vec2 );

        }else if(prefix == "vn"){  //  vertex normal

            ss >> tmp_vec3.x >> tmp_vec3.y >> tmp_vec3.z;
            normalVertex.push_back( tmp_vec3 );

        }else if(prefix == "usemtl"){   //  use material
            
            ss >> materialName;

        }else if(prefix == "f"){    //  face

            while( ss.rdbuf()->in_avail() ){

                if( !renderType )
                    vertexPerFace++;
                
                ss >> tmp_int;
                positionIndex.push_back(tmp_int);

                if( ss.peek() == '/' ){

                    ss.ignore(1, '/');
                    ss >> tmp_int;
                    if( textcoordVertex.size() > 0 )
                        textcoordIndex.push_back(tmp_int);
                    else if(normalVertex.size() > 0)
                        normalIndex.push_back(tmp_int);

                    if( ss.peek() == '/' ){
                        ss.ignore(1, '/');
                        ss >> tmp_int;
                        normalIndex.push_back(tmp_int);
                    }
                }

            }

            if( !renderType ){
                switch (vertexPerFace){
                    case 1:
                        renderType = GL_POINTS;
                        break;
                    case 2:
                        renderType = GL_LINES;
                        break;
                    case 3:
                        renderType = GL_TRIANGLES;
                        break;
                    case 4:
                        renderType = GL_QUADS;
                    default:
                        break;
                }
            }

        }

    }

    meshes.push_back( new Mesh(positionVertex, textcoordVertex, normalVertex, positionIndex, textcoordIndex, normalIndex, renderType ) );

    if(meshes.size()>0){
        if( materialMap->find(materialName) != materialMap->end() )
            models.push_back( new Model( name, meshes, (*materialMap)[materialName] ) );
        else
            models.push_back( new Model( name, meshes, (*materialMap)["default"] ) );
    }

    for(Model* &m : models)
        m->normalize(maxVertex);

    return models;

}
