#pragma once

#include <vector>

#include "Shader.hpp"
#include "Entitie.hpp"

#include "Autoroute.hpp"
#include "Ecosystem.hpp"

#include "Camera.hpp"

class GraphicEnvironement{


public:
	//Initialise toutes les entités en foncition des voitures et des noeud
	GraphicEnvironement(std::vector<Autoroute::Node> const& allNodeInfo, std::vector<Car> const& allCars, Camera* cam);

	//Fonction qui va appeller draw sur toutes les entité dans les vecteurs
	void draw() const;

	//Fonction qui update la partie graphique en fonction des voitures de la partie logique
	void update(std::vector<Car> const& allCars);

	//Fonction qui gère la touche espace pour eclater la vue pour essayer de mieux comprendre les intersection de noeud
	void switchView();


private:

	Shader mainShader_;

	Camera* cam_;

	bool explodedView_;
	bool inTransition_;

	Entitie terrain_;
	std::vector<Entitie> allNode_;


	std::vector<Entitie> allPath_;
	std::vector<float> allDelta_;


	std::vector<Entitie> allCars_;

	
};