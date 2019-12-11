
/*

	TP autoroute de Guillaume Magniadas

	Note : La classe Shader et la classe Camera ne sont pas de moi,
		   elles ont été récupéré dans le cours d'openGL de developpez.net
	
	Note : La partie graphique n'a pas dutout été optimisé, on pourrait l'optimiser en ne faisant qu'un seul array buffer pour tout les cube de noeud par exemple.
	
	Utiliser :
	
	ZQSD pour se déplacer et la sourie pour bouger la caméra

*/



#include <iostream>

///Logique
#include "Ecosystem.hpp"

///Graphique
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "GraphicEnvironement.hpp"


#include "Camera.hpp"



#define NB_VOITURE 10

#define NB_NODE 30

GLFWwindow* window_;

#define WIDTH 	800
#define HEIGHT 	600



static Camera myCam(glm::vec3(0.0f, 10.0f, 0.0f));
static float deltaTime;

float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;


GraphicEnvironement* myEnviroPtr;


//Fonction qui gère les déplacements de la sourie
void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    if(firstMouse){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // inversé car va de bas en haut

    lastX = xpos;
    lastY = ypos;

    myCam.ProcessMouseMovement(xoffset, yoffset);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS){
    	myEnviroPtr->switchView();
    }

}


void startGraphic(){


	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif



	window_ = glfwCreateWindow(WIDTH, HEIGHT, "Ecosystem Autoroute", NULL, NULL);

	if (window_ == NULL){
	    std::cout << "Failed to create GLFW window" << std::endl;
	    glfwTerminate();
	    exit(1);
	}

	glfwMakeContextCurrent(window_);
	glfwSetCursorPosCallback(window_, mouse_callback);
	glfwSetKeyCallback(window_, key_callback);

	//Désactiver la sourie
	glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	//FPS 60
	glfwSwapInterval(1);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(2);
	}
		


}


//Fonction qui gère les input de deplacement de la camera
void processInput(GLFWwindow *window){

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
	}

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        myCam.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        myCam.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        myCam.ProcessKeyboard(LEFT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        myCam.ProcessKeyboard(RIGHT, deltaTime);
    }

}



int main(int argc, char const *argv[]){

	srand(time(NULL));

	startGraphic();

	Ecosystem myEco(NB_NODE, NB_VOITURE);
	GraphicEnvironement myEnviro(myEco.getNodeInfo(), myEco.getCarInfo(), &myCam);
	myEnviroPtr = &myEnviro;

	myEco.start();
	
	float lastFrame;

	while(!glfwWindowShouldClose(window_)){

		float time = glfwGetTime();

		//Temps, utile seulement dans les déplacement et la camera
		deltaTime = time - lastFrame;
		lastFrame = time;

		processInput(window_);

		//Clear
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//On update la partie graphique pour on la dessine
		myEnviro.update(myEco.getCarInfo());
		myEnviro.draw();


		glfwSwapBuffers(window_);
		glfwPollEvents();

	}


	//On detache tout les thread quand on fini car les thread n'ont pas forcément tous fini
	myEco.detachAll();


	return 0;
}











