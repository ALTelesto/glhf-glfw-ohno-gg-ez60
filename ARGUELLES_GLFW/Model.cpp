#include "Model.hpp"

Model3D::Model3D(std::string pathModel, std::string pathTexture, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, glm::vec3 color) {
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;
    this->color = color;
    this->specStr = 0;
    this->specPhong = 0;

    //std::string path = "3D/djSword.obj";
    std::vector<tinyobj::shape_t> shape;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    tinyobj::attrib_t attributes;

    if (!pathModel.empty()) {
        std::cout << "invalid path" << std::endl;
        return;
    }

    bool success = tinyobj::LoadObj(
        &attributes,
        &shape,
        &material,
        &warning,
        &error,
        pathModel.c_str()
    );

    if (!success) {
        std::cout << "failed loading model" << std::endl;
        return;
    }

    for (int i = 0; i < shape[0].mesh.indices.size(); i++) {
        tinyobj::index_t vData = shape[0].mesh.indices[i];
        //X
        fullVertexData.push_back(
            attributes.vertices[vData.vertex_index * 3]
        );
        //Y
        fullVertexData.push_back(
            attributes.vertices[vData.vertex_index * 3 + 1]
        );
        //Z
        fullVertexData.push_back(
            attributes.vertices[vData.vertex_index * 3 + 2]
        );

        fullVertexData.push_back(
            attributes.normals[vData.normal_index * 3]
        );
        fullVertexData.push_back(
            attributes.normals[vData.normal_index * 3 + 1]
        );
        fullVertexData.push_back(
            attributes.normals[vData.normal_index * 3 + 2]
        );

        //U
        fullVertexData.push_back(
            attributes.texcoords[vData.texcoord_index * 2]
        );
        //V
        fullVertexData.push_back(
            attributes.texcoords[vData.texcoord_index * 2 + 1]
        );

    }

    if (!pathTexture.empty()) {
        std::cout << "invalid texture path" << std::endl;
        return;
    }
    else {
        int img_width, img_height, colorChannels;

        stbi_set_flip_vertically_on_load(true);
        unsigned char* tex_bytes = stbi_load(pathTexture.c_str(), &img_width, &img_height, &colorChannels, 0);

        glGenTextures(1, &texture);

        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_2D, texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_bytes);

        glGenerateMipmap(GL_TEXTURE_2D);

        glEnable(GL_DEPTH_TEST);

        stbi_image_free(tex_bytes);
    }


    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GLfloat) * fullVertexData.size(),
        fullVertexData.data(),
        // attributes.vertices.data(),
        GL_DYNAMIC_DRAW);

    //model
    glVertexAttribPointer(
        0,
        3, //xyz
        GL_FLOAT,
        GL_FALSE,
        //XYZ UV
        8 * sizeof(GL_FLOAT),
        (void*)0
    );

    //normal
    GLintptr normalPtr = 3 * sizeof(float);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(GL_FLOAT),
        (void*)normalPtr
    );

    //texture
    GLintptr uvPtr = 6 * sizeof(float);
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        //XYZ UV
        8 * sizeof(GL_FLOAT),
        (void*)uvPtr
    );

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

}

// code was ripped from the while() loop in main() and placed into draw() function
Model3D::void draw(GLuint& shaderProgram) {

    glUseProgram(shaderProgram);

    glm::mat4 identity_matrix = glm::mat4(1.0f);

    // applying of model transformation
    // translate -> rotate -> scale
    glm::mat4 transformation_matrix = glm::translate(glm::mat4(1.0f), this->position);

    //glm::vec3 relativePosition = this->position - glm::vec3(0.f,0.f,0.f);
    float phi = glm::radians(this->rotation.x);
    float theta = glm::radians(this->rotation.z);
    glm::vec3 relativePosition = glm::vec3(
        
    );

    transformation_matrix = glm::rotate(transformation_matrix, glm::radians(this->rotation.x), glm::vec3(1, 0, 0));
    transformation_matrix = glm::rotate(transformation_matrix, glm::radians(this->rotation.y), glm::vec3(0, 1, 0));
    transformation_matrix = glm::rotate(transformation_matrix, glm::radians(this->rotation.z), glm::vec3(0, 0, 1));
    transformation_matrix = glm::translate(transformation_matrix, relativePosition);

    transformation_matrix = glm::scale(transformation_matrix, this->scale);
    // end of applying of model transformation 

    // sending transformation matrix to shader program
    unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

    //set texture
    GLuint tex0Address = glGetUniformLocation(shaderProgram, "tex0");
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glUniform1i(tex0Address, 0);

    //set color
    unsigned int colorLoc = glGetUniformLocation(shaderProgram, "FragColorIn");
    glUniform4fv(colorLoc, 1, glm::value_ptr(glm::vec4(color, 1.f)));
    // end of sending stuff to shader program

    // rendering
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 8);
    // end

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Model3D::setPosition(glm::vec3 position) {
    this->position = position;
}
void Model3D::setRotation(glm::vec3 rotation) {
    this->rotation = rotation;
}
void Model3D::setScale(glm::vec3 scale) {
    this->scale = scale;
}
void Model3D::setColor(glm::vec3 color) {
    this->color = color;
}
void Model3D::setSpec(glm::vec2 spec) {
    this->specStr = spec.x;
    this->specPhong = spec.y;
}

glm::vec3 Model3D::getPosition() {
    return this->position;
}
glm::vec3 Model3D::getRotation() {
    return this->rotation;
}
