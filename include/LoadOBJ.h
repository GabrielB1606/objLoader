#pragma once

static std::vector<Vertex> LoadOBJ(const char* filename){
    
    // vertex data
    std::vector<glm::vec3> position;
    std::vector<glm::vec2> textcoord;
    std::vector<glm::vec3> normal;

    // vertex indices
    std::vector<GLint> positionIndex;
    std::vector<GLint> textcoordIndex;
    std::vector<GLint> normalIndex;

    // vector of complete vertices
    std::vector<Vertex> vertexArray;

    // file variables
    std::stringstream ss;
    std::ifstream file(filename);
    std::string line = "";

    // line parse variables    
    std::string prefix = "";
    glm::vec3 tmp_vec3;
    glm::vec2 tmp_vec2;
    GLint tmp_int;


    if( !file.is_open() )
        std::cout << "ERROR: could not open OBJ: " << filename << "\n";

    while( std::getline(file, line) ){


        ss.clear();
        ss.str(line);

        ss >> prefix;

        if(prefix == "v"){  //  vertex position

            ss >> tmp_vec3.x >> tmp_vec3.y >> tmp_vec3.z;
            position.push_back( tmp_vec3 );

        }else if(prefix == "s"){    //  smoothing group

        }else if(prefix == "vt"){   //  textcoord
            
            ss >> tmp_vec2.x >> tmp_vec2.y;
            textcoord.push_back( tmp_vec2 );

        }else if(prefix == "vn"){  //  vertex normal

            ss >> tmp_vec3.x >> tmp_vec3.y >> tmp_vec3.z;
            normal.push_back( tmp_vec3 );

        }else if(prefix == "usemtl"){   //  use material

        }else if(prefix == "f"){    //  face

            for(char i = 0; i<3; i++){

                ss >> tmp_int;
                positionIndex.push_back(tmp_int);

                if( ss.peek() == '/' ){

                    ss.ignore(1, '/');
                    ss >> tmp_int;
                    textcoordIndex.push_back(tmp_int);

                    if( ss.peek() == '/' ){
                        ss.ignore(1, '/');
                        ss >> tmp_int;
                        normalIndex.push_back(tmp_int);
                    }
                }

            }

        }

    }

    // done reading
    vertexArray.resize( positionIndex.size(), Vertex() );

    for (size_t i = 0; i < vertexArray.size(); i++){
        vertexArray[i].position = position[ positionIndex[i]-1 ];
        vertexArray[i].textcoord = textcoord[ textcoordIndex[i]-1 ];
        vertexArray[i].normal = normal[ normalIndex[i]-1 ];
        vertexArray[i].color = glm::vec3(0.7f);
    }

    return vertexArray;

}
