//STANDARD
#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<fstream>
#include <string>
#include <vector>
#include<sstream>

//GLEW
#include<GL\glew.h>

//GLFW
#include<GLFW\glfw3.h>

//OPENGL MATH GLM
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Vertex.h"
#include "Material.h"
#include "Primitive.h"
#include "Mesh.h"
#include "Model.h"
#include "PrimitiveQuad.h"
#include "PrimitivePyramid.h"
#include "Camera.h"
#include "LoadOBJ.h"
#include "Game.h"

using namespace std;