
#include "Autoroute.hpp"

#include <iostream>

//Ne pas mettre en dessous de 2.
#define DISTANCE_ENTRE_POINT 7

///NODE
Autoroute::Node::Node(size_t id, int x, int y) : id_(id), x_(x), y_(y), nearDone_(false), taken_(false) {}

Autoroute::Node::Node(Node const& copied) : id_(copied.id_), x_(copied.x_), y_(copied.y_), nearDone_(copied.nearDone_), taken_(copied.taken_), nextNode_(copied.nextNode_) {}

Autoroute::Autoroute(){

}

bool Autoroute::find(int x, int y){

	if(worldPos_.find(x) != worldPos_.end()){
		if(worldPos_[x].find(y) != worldPos_[x].end()){
			return true;
		}
	}

	return false;
}


//Fonction simple qui genere tout les noeuds et toutes les routes
/*
On pourrait la remplacer par une fonction qui lit un fichier qui contient les noeud et leur enfant,
la partie graphique n'en sera pas impacté.
*/
void Autoroute::generateRoads(size_t nbNode){

	if(nbNode == 0){
		std::cerr << "ERROR : You need at least 1 node" << std::endl;
		return;
	}

	allNode_.reserve(nbNode);

	//First node at x = 0 and y = 0
	allNode_.emplace_back(0, 0, 0);
	worldPos_[0][0] = 0;


	for(size_t i(1); i < nbNode; ++i){

		size_t parentNode;

		while(true){
		
			parentNode = rand()%allNode_.size();
			if(allNode_[parentNode].nextNode_.size() < 4 && allNode_[parentNode].nearDone_ == false){
				break;
			}
		
		}

		Node& parentNodeRef(allNode_[parentNode]);

		int parentX(parentNodeRef.x_), parentY(parentNodeRef.y_);



		size_t newNodeX, newNodeY;
		newNodeX = (rand()%DISTANCE_ENTRE_POINT - DISTANCE_ENTRE_POINT/2) + parentX;
		newNodeY = (rand()%DISTANCE_ENTRE_POINT - DISTANCE_ENTRE_POINT/2) + parentY;

		if(find(newNodeX, newNodeY)){
			--i;
			continue;
		}

		//A partir d'ici parentX et parentY valent les nouvellent coordonnée de l'enfant
		allNode_.emplace_back(i, newNodeX, newNodeY);
		worldPos_[newNodeX][newNodeY] = i;

		//Relier le nouveau noeud à son père
		allNode_.back().nextNode_.emplace_back(&parentNodeRef);

		//Relier le père au nouveau noeud
		parentNodeRef.nextNode_.emplace_back(&allNode_.back());


	}


}

//On part du principe qu'on lance cette fonction seulement au début quand les thread ne tourne pas donc pas besoins d'activer les mutex.
//Si on décide de pouvoir rajouter des voitures au plein millieu du programme, on devrai rajouter un locker comme pour stopPoint.
void Autoroute::startPoint(size_t nodeIndex){
	allNode_[nodeIndex].taken_ = true;
}

void Autoroute::stopPoint(size_t nodeIndex){

	std::unique_lock lock(allNode_[nodeIndex].blocked_);

	allNode_[nodeIndex].taken_ = false;

}

//Fonction lancé par les voiture pour choisir leur prochain noeud
size_t Autoroute::pickRandomNext(size_t actualNode, size_t carIndex){

	//On récupère le chrono now
	std::chrono::steady_clock::time_point begin(std::chrono::steady_clock::now());

	//On récupère le nombre d'enfant possible sur le noeud actuel
	size_t nbChild(allNode_[actualNode].nextNode_.size());

	//On choisi un index aléatoire entre 0 et le nombre
	size_t chosenNextIndex(rand()%nbChild);

	//Pointeur sur le prochain noeud, choisi avec chosenNextIndex
	Node* chosenNextPtr = allNode_[actualNode].nextNode_[chosenNextIndex];


	//On essaie d'acceder au au boolean du prochain noeud, vue que c'est une variable qui risque d'être modifié, on utilise un mutex
	std::unique_lock lock(chosenNextPtr->blocked_);

	while(chosenNextPtr->taken_){

		//On n'accede plus au boolean, on dévérouille
		lock.unlock();

		//Si le temps dépasse 4 secondes et qu'il n'y à pas qu'un seul chemin
		if(std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::steady_clock::now() - begin).count() > 4000 && nbChild > 1){

			size_t newNextNodeIndex;

			//Ce do while permet de choisir un nouveau point qui n'est pas l'ancien point
			do{
				newNextNodeIndex = rand()%nbChild;
			}
			while(newNextNodeIndex == chosenNextIndex);

			chosenNextIndex = newNextNodeIndex;

			chosenNextPtr = allNode_[actualNode].nextNode_[chosenNextIndex];
			
			std::cout <<"La voiture : " << carIndex <<  " : Plus de 4 secondes sont passé, je change de direction, nouvelle direction : " << chosenNextPtr->id_ << std::endl;
		
		}

		//on dort de 1 à 3 secondes
		int waitingTime(rand()%3 + 1);

		std::cout << "La voiture : " << carIndex <<  " Le noeud " << chosenNextPtr->id_ << " est pris, je dors " << waitingTime << " secondes." << std::endl;

		std::this_thread::sleep_for(std::chrono::milliseconds(waitingTime*1000));


		//On reverouille car on va reacceder au boolean
		lock.lock();

	}

	//On laisse le premier lock vérouillé car on va acceder au boolean
	//Et on vérouille aussi le mutex de l'ancien noeud courant car on va le modifier.
	std::unique_lock lockCurrent(allNode_[actualNode].blocked_);

	allNode_[actualNode].taken_ = false;
	chosenNextPtr->taken_ = true;

	//On retourne le nouvel id
	return chosenNextPtr->id_;

}

void Autoroute::print(){

	for(size_t i(0), maxSize(allNode_.size()); i < maxSize; ++i){

		std::cout << "Noeud : " << allNode_[i].id_ << " Pos X : " << allNode_[i].x_ << " Y : " << allNode_[i].y_ << std::endl << "Next Possible : ";

		for(size_t j(0); j < allNode_[i].nextNode_.size(); ++j){

			std::cout << allNode_[i].nextNode_[j]->id_ << " - ";

		}
		std::cout << std::endl;

	}

}

std::vector<Autoroute::Node> const& Autoroute::getNodeInfo() const {
	return allNode_;
}



