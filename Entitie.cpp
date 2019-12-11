
#include "Entitie.hpp"

Entitie::Entitie(Shader* shader, Camera* cam) : visible_(true), cam_(cam), shader_(shader), scale_(1.0f, 1.0f, 1.0f), rotationMat_(1.0f){

    ///EN ATTENDENT
    vertices_ = {
        // positions          // normals           // texture coords
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
        1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        
        1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
        1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    
    
    glGenVertexArrays(1, &VAO_);
    glBindVertexArray(VAO_);
    
    
    glGenBuffers(1, &VBO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(float), vertices_.data(), GL_STATIC_DRAW);
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    //Vecteur normal des sommets
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

}

glm::vec3 const& Entitie::getPosition(){
    return position_;
}

void Entitie::setPosition(float x, float y, float z){
    position_.x = x;
    position_.y = y;
    position_.z = z;
}

void Entitie::setSize(float x, float y, float z){
    scale_.x = x;
    scale_.y = y;
    scale_.z = z;
}

void Entitie::setRotation(glm::mat4& rotationMat){
    rotationMat_ = std::move(rotationMat);
}

void Entitie::setColor(float r, float g, float b){
    color_.r = r;
    color_.g = g;
    color_.b = b;
}


void Entitie::draw() const {

    if(!visible_) return;

	shader_->use();
    
	//glm::vec3 position(0, 0, -10);
    glm::mat4 model(glm::translate(glm::mat4(1.0f), position_));
    model = model * rotationMat_;
    model = glm::scale(model, scale_);
    
    
    shader_->setMat4("model", model);
    shader_->setMat3("modelNormal", glm::mat3(glm::transpose(glm::inverse(model))));

    ////TOUT LES NEED
    shader_->use();
    
    shader_->setVec3("material.diffuse", color_);
    shader_->setVec3("material.specular", color_);
    shader_->setFloat("material.shininess", 32.0f);
    
    shader_->setVec3("selectMaterial.diffuse", 0.8, 0.2, 0.2);
    shader_->setVec3("selectMaterial.specular", 0.7, 0.5, 0.5);
    shader_->setFloat("selectMaterial.shininess", 32.0f);
    
    
    shader_->setVec3("light.ambient",  0.3f, 0.3f, 0.3f);
    shader_->setVec3("light.diffuse",  1.f, 1.f, 1.f); // assombri un peu la lumière pour correspondre à la scène
    shader_->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    
    shader_->setVec3("light.direction", 0.f, -1.0f, 0.f);
    
    shader_->setFloat("light.constant",  1.0f);
    shader_->setFloat("light.linear",    0.09f);
    shader_->setFloat("light.quadratic", 0.032f);
    
    
    
    
    shader_->setVec3("light.position", glm::vec3(0, 100, 0));
    
    
    
    //Calcule camera

    glm::mat4 view(cam_->GetViewMatrix());
    //glm::mat4 view(glm::lookAt(glm::vec3(0, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
    
    shader_->setMat4("view", view);
    
    //Position de la cam
    shader_->setVec3("viewPos", glm::vec3(0, 20, 20));
    
    shader_->setMat4("projection", glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 500.0f));
    
    //Bug
    glBindVertexArray(VAO_);
    
    glDrawArrays(GL_TRIANGLES, 0, 36);

}





