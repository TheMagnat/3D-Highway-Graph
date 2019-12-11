
#include "Ecosystem.hpp"

#include <set>
#include <iostream>

Car::Car(size_t actualIndex) : actualIndex_(actualIndex), isMoving_(false), finished_(false) {}

Ecosystem::Ecosystem(size_t nbNode, size_t nbCars){


	myRoute_.generateRoads(nbNode);
	myRoute_.print();

	std::set<size_t> alreadyDone;

	for(size_t i(0); i < nbCars; ++i){

		size_t selectedNode;

		while(true){

			selectedNode = rand()%nbNode;

			if(alreadyDone.count(selectedNode) == 0){

				alreadyDone.emplace(selectedNode);
				break;

			}
		
		}

		allCars_.emplace_back(selectedNode);

	}

}

void Ecosystem::workingCar(size_t fuel, size_t index){

	while(fuel){

		//On trouve un nouveau point
		//pickRandomNext est la fonction principale pour piocher un nouveau noeud sur l'autoroute
		size_t newNodeIndex(myRoute_.pickRandomNext(allCars_[index].actualIndex_, index));

		std::cout << "La voiture : " << index << " se déplace de " << allCars_[index].actualIndex_ << " à " << newNodeIndex << std::endl;

		//On met le nouveau point dans le prochain point de la voiture
		allCars_[index].isMoving_ = true;
		allCars_[index].nextIndex_ = newNodeIndex;
			
		//On attend un certain temps en millisecondes
		int waitingTime((rand()%3 + 1) * 1000);
		allCars_[index].timeStartMoving_ = std::chrono::steady_clock::now();
		allCars_[index].timeToMove_ = waitingTime;
		std::this_thread::sleep_for(std::chrono::milliseconds(waitingTime));
		
		//On met ce nouveau point dans le point actuel de la voiture
		allCars_[index].actualIndex_ = allCars_[index].nextIndex_;
		allCars_[index].isMoving_ = false;

		//On enlève de l'essence
		--fuel;
	
	}

	//Quand un thread n'a plus d'essence on libère le point
	myRoute_.stopPoint(allCars_[index].actualIndex_);

	//On met la voiture sur fini 
	allCars_[index].finished_ = true;

	std::cout << "La voiture : " << index << " n'a plus d'essence" << std::endl;

}

void Ecosystem::start(){

	//On reserve la bonne taille pour le vecteur de thread
	allThread_.reserve(allCars_.size());

	//On met tout les points de depart sur occupé
	for(size_t i(0); i < allCars_.size(); ++i){

		std::cout << "Voiture a la case : " << allCars_[i].actualIndex_ << std::endl;
		myRoute_.startPoint(allCars_[i].actualIndex_);

	}

	//On commence tout les thread
	for(size_t i(0); i < allCars_.size(); ++i){

		allThread_.emplace_back(&Ecosystem::workingCar, this, 10, i);

	}


}

void Ecosystem::joinAll(){
	for(size_t i(0); i < allCars_.size(); ++i){
		allThread_[i].join();
	}
}

void Ecosystem::detachAll(){
	for(size_t i(0); i < allCars_.size(); ++i){
		allThread_[i].detach();
	}
}

std::vector<Car> const& Ecosystem::getCarInfo() const {
	return allCars_;
}

std::vector<Autoroute::Node> const& Ecosystem::getNodeInfo() const {
	return myRoute_.getNodeInfo();
}

