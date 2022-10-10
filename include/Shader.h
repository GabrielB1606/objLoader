#pragma once

class Shader{
    private:
        GLuint id;
        
        const int versionMaj;
        const int versionMin;

        const char* glsl_version;

        std::string loadShaderSource(char* filename);
        GLuint loadShader(GLenum type, char* filename);
        bool linkProgram(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader);

    public:
        Shader(const char* glsl_version, const int versionMaj, const int versionMin, char* vertexFile, char* fragmentFile, char* geometryFile);
        ~Shader();
        void use();
        void stopUsing();
        void setVec2f(glm::vec2 value, const GLchar* name);
        void setVec3f(glm::vec3 value, const GLchar* name);
        void setVec4f(glm::vec4 value, const GLchar* name);
        void setMat4fv(glm::mat4 value, const GLchar* name, GLboolean transpose);
        void setMat3fv(glm::mat3 value, const GLchar* name, GLboolean transpose);
        void set1f(GLfloat value, const GLchar* name);
};

Shader::Shader(const char* glsl_version, const int versionMaj, const int versionMin,char* vertexFile, char* fragmentFile, char* geometryFile = nullptr) : versionMaj(versionMaj), versionMin(versionMin){

    this->glsl_version = glsl_version;

    GLuint vertexShader = 0;
    GLuint fragmentShader = 0;
    GLuint geometryShader = 0;

    vertexShader = loadShader(GL_VERTEX_SHADER, vertexFile);
    
    if( geometryFile != nullptr)
        geometryShader = loadShader(GL_GEOMETRY_SHADER, geometryFile);

    fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentFile);

    this->linkProgram( vertexShader, fragmentShader, geometryShader );

    // end
    glDeleteShader(vertexShader);
    if(geometryShader)
        glDeleteShader(geometryShader);
    glDeleteShader(fragmentShader);

}

Shader::~Shader(){
    glDeleteProgram(this->id);
}

void Shader::use(){
    glUseProgram(this->id);
}

void Shader::stopUsing(){
    glUseProgram(0);
}

void Shader::setVec3f(glm::vec3 value, const GLchar* name){
    this->use();
    
    glUniform3fv( glGetUniformLocation(this->id, name), 1, glm::value_ptr(value) );

    this->stopUsing();
}

void Shader::setVec2f(glm::vec2 value, const GLchar* name){
    this->use();
    
    glUniform2fv( glGetUniformLocation(this->id, name), 1, glm::value_ptr(value) );

    this->stopUsing();
}

void Shader::setVec4f(glm::vec4 value, const GLchar* name){
    this->use();
    
    glUniform4fv( glGetUniformLocation(this->id, name), 1, glm::value_ptr(value) );

    this->stopUsing();
}

void Shader::setMat3fv(glm::mat3 value, const GLchar* name, GLboolean transpose = GL_FALSE){
    this->use();
    
    glUniformMatrix3fv(glGetUniformLocation(this->id, name), 1, transpose, glm::value_ptr(value));

    this->stopUsing();
}

void Shader::setMat4fv(glm::mat4 value, const GLchar* name, GLboolean transpose = GL_FALSE){
    this->use();
    
    glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, transpose, glm::value_ptr(value));

    this->stopUsing();
}

void Shader::set1f(GLfloat value, const GLchar* name){
    this->use();
    glUniform1f( glGetUniformLocation(this->id, name), value );
    this->stopUsing();
}

std::string Shader::loadShaderSource(char* filename){
    
    std::string temp = "";
    std::string src = "";
    std::ifstream in_file;

    in_file.open(filename);

    if(in_file.is_open()){

        while( std::getline(in_file, temp) ){
            src += temp + "\n";
        }

    }else{
        std::cout << "ERROR -> Can't open Shader Source\n\tShader src path: " << filename << "\n";
        return "";
    }

    in_file.close();

    std::string version = glsl_version;
    src.replace( src.find("#version"), 12, "#version "+version );

    return src;
}

GLuint Shader::loadShader(GLenum type, char* filename){

    char infoLog[512];
    GLint success;

    GLuint shader = glCreateShader( type );

    std::string str_src =  this->loadShaderSource(filename);
    const GLchar* src = str_src.c_str();

    glShaderSource( shader, 1, &src, NULL );
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if( !success ){
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR -> Can't compile shader:\n\tShader src path: " << filename <<"\n\t" << infoLog << "\n";
        return -1;
    }

    return shader;
}

bool Shader::linkProgram(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader){
    char infoLog[512];
    GLint success;

    this->id = glCreateProgram();
    
    glAttachShader(this->id, vertexShader);

    if(geometryShader)
        glAttachShader(this->id, geometryShader);

    glAttachShader(this->id, fragmentShader);

    glLinkProgram(this->id);

    glGetProgramiv(this->id, GL_LINK_STATUS, &success);
    if( !success ){
        glGetProgramInfoLog(this->id, 512, NULL, infoLog);
        std::cout << "ERROR -> Program couldn't link:\n\t" << infoLog << "\n";
        return false;
    }

    glUseProgram(0);
    return true;
}