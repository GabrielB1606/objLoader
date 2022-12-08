class Texture{
private:
    
    GLuint texture;
    GLenum target;
    int width, height, nrChannels;
    GLuint texUnit;

public:
    Texture(std::string filename, GLenum target, GLuint texUnit);
    ~Texture();

    void bind();
    void unbind();

    GLuint getID() { return texture; }

    void sendToShader(Shader* program);
};

void Texture::sendToShader(Shader* program){
    glActiveTexture(texUnit);
    glBindTexture( this->target, this->texture );
    GLuint i = texUnit - GL_TEXTURE0;
    program->set1i( i , texture_names[i].c_str() );
}

void Texture::bind(){
    glBindTexture( this->target, this->texture );
}

void Texture::unbind(){
    // glBindTexture( this->target, 0 );
    // glActiveTexture(0);
}

Texture::Texture(std::string filename, GLenum target, GLuint texUnit){

    this->target = target;
    this->texUnit = texUnit;

    // glActiveTexture(texUnit); // activate the texture unit first before binding texture
    glGenTextures(1, &this->texture);
    glBindTexture(this->target, this->texture);
    
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(this->target, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(this->target, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(this->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(this->target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // load and generate the texture
    
    unsigned char *data = stbi_load(filename.c_str() , &this->width, &this->height, &this->nrChannels, 0);
    if (data){

        glTexImage2D(this->target, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(this->target);
    
    }else{
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);
    glBindTexture(this->target, 0);

}

Texture::~Texture(){

    glDeleteTextures(1, &this->texture);

}
