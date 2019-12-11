
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"

#include "Camera.hpp"

#include <vector>


class Entitie{

public:
	Entitie(Shader* shader, Camera* cam);

	glm::vec3 const& getPosition();

	void setPosition(float x, float y, float z);
	void setSize(float x, float y, float z);
	void setRotation(glm::mat4& rotationMat);

	void setColor(float r, float g, float b);

	void draw() const;


	//Variable publique
	bool visible_;

private:

	Shader* shader_;

	Camera* cam_;


	std::vector<float> vertices_;

	unsigned int VAO_;
    unsigned int VBO_;

    glm::vec3 color_;
    glm::vec3 position_;
    glm::vec3 scale_;

    glm::mat4 rotationMat_;
	
};