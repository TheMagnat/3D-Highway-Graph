#pragma once

#include "Autoroute.hpp"


//Structure qui stoque toute les info utiles pour une voiture
struct Car{

	Car(size_t actualIndex);

	//Index du noeud actuel
	size_t actualIndex_;

	//Si la voiture est entrain de bouger (dormir)
	bool isMoving_;
	
	//Une clock qui stoque le temps quand le thread commence à bouger (dormir)
	std::chrono::steady_clock::time_point timeStartMoving_;
	//Stoque le temps que le thread dort
	float timeToMove_;

	//Index du prochain noeud après le sleep
	size_t nextIndex_;

	//Si le thread n'a plus d'essence
	bool finished_;

};

/*
Classe qui gère toute la partie logique
*/

class Ecosystem{

public:

	Ecosystem(size_t nbNode, size_t nbCars);

	//Fonction principale des thread
	void workingCar(size_t fuel, size_t index);

	//Fonction qui démarre les threads
	void start();

	//Join tout les thread
	void joinAll();

	//Detach tout les thread
	void detachAll();

	//Fonction qui recupère une référence sur le vecteur de voiture (Utile pour la partie graphique seulement)
	std::vector<Car> const& getCarInfo() const;

	//Fonction qui recupère une référence sur le vecteur de noeud de l'autoroute (Pour la partie graphique)
	std::vector<Autoroute::Node> const& getNodeInfo() const;


private:

	Autoroute myRoute_;

	std::vector<Car> allCars_;


	std::vector<std::thread> allThread_;
	
};