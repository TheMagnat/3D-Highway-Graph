
#include "GraphicEnvironement.hpp"

GraphicEnvironement::GraphicEnvironement(std::vector<Autoroute::Node> const& allNodeInfo, std::vector<Car> const& allCars, Camera* cam) : mainShader_("Shaders/entity.vs", "Shaders/entity.fs"), cam_(cam), explodedView_(false), inTransition_(false), terrain_(&mainShader_, cam) {

	allNode_.reserve(allNodeInfo.size());

	for(size_t i(0); i < allNodeInfo.size(); ++i){

		allNode_.emplace_back(&mainShader_, cam);
		allNode_.back().setPosition(allNodeInfo[i].x_, -1, allNodeInfo[i].y_);
		allNode_.back().setColor(0.0f, 0.0f, 1.0f);
		allNode_.back().setSize(0.5f * 0.3f, 0.5f * 0.3f, 0.5f * 0.3f);


		for(size_t j(0), maxSize2(allNodeInfo[i].nextNode_.size()); j < maxSize2; ++j){

			if(i > allNodeInfo[i].nextNode_[j]->id_){
				continue;
			}


			float dist(sqrt(pow(allNodeInfo[i].x_ - allNodeInfo[i].nextNode_[j]->x_, 2) + pow(allNodeInfo[i].y_ - allNodeInfo[i].nextNode_[j]->y_, 2)));

			///Point du triangle
			glm::vec2 p3(allNodeInfo[i].x_, allNodeInfo[i].nextNode_[j]->y_);

			//On met un moins car le repère est inversé en openGL
			float angle(atan2(-(allNodeInfo[i].y_ - allNodeInfo[i].nextNode_[j]->y_), allNodeInfo[i].x_ - allNodeInfo[i].nextNode_[j]->x_));


			glm::mat4 rotateMat(glm::rotate(glm::mat4(1.0), angle, glm::vec3(0.0f, 1.0f, 0.0f)));

			allPath_.emplace_back(&mainShader_, cam);
			allPath_.back().setPosition((allNodeInfo[i].x_ + allNodeInfo[i].nextNode_[j]->x_)/2.0f, -1, (allNodeInfo[i].y_ + allNodeInfo[i].nextNode_[j]->y_)/2.0f);
			allPath_.back().setColor(0.0f, 1.0f, 0.0f);
			allPath_.back().setSize(dist/2.0f, 0.25f * 0.3f, 0.25f * 0.3f);

			allPath_.back().setRotation(rotateMat);

			allDelta_.emplace_back(0);

		}

	}


	allCars_.reserve(allCars.size());
	for(size_t i(0); i < allCars.size(); ++i){

		allCars_.emplace_back(&mainShader_, cam);
		allCars_.back().setPosition(allNodeInfo[allCars[i].actualIndex_].x_, 0, allNodeInfo[allCars[i].actualIndex_].y_);
		allCars_.back().setColor(1.0f, 0.0f, 0.0f);
		allCars_.back().setSize(0.33f * 0.3f, 0.33f * 0.3f, 0.33f * 0.3f);

	}




}

void GraphicEnvironement::update(std::vector<Car> const& allCars){

	//On update la position des voitures
	for(size_t i(0); i < allCars.size(); ++i){

		if(allCars[i].finished_){
			//Si la voiture n'est pas visible on met le boolean à false pour ne plus la déssiner dans la phase de dessin
			allCars_[i].visible_ = false;
			continue;
		}



		Entitie& nodeEntitie = allNode_[allCars[i].actualIndex_];

		glm::vec3 nodePosition = nodeEntitie.getPosition();

		if(allCars[i].isMoving_){
			
			Entitie& nextNodeEntitie = allNode_[allCars[i].nextIndex_];
			glm::vec3 nextNodePosition = nextNodeEntitie.getPosition();

			//Calcule d'interpolation en fonction du temps actuel, du temps de depart et du temps max pour trouver la position de la voiture pour avoir une transition fluide.
			float spentTime(std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::steady_clock::now() - allCars[i].timeStartMoving_).count());

			float delta(spentTime / allCars[i].timeToMove_);

			glm::vec3 newPosition(nodePosition * (1 - delta) + nextNodePosition * delta);

			//-7 est une bonne valeur dans mon graphe pour la hauteur
			allCars_[i].setPosition(newPosition.x, -0.7, newPosition.z);

		}
		else{
			allCars_[i].setPosition(nodePosition.x, -0.7, nodePosition.z);	
		}
		


	}

	//Trasition
	if(!inTransition_) return;

	//Transition pour la vue éclaté
	float toAdd(-1 + explodedView_*2);
	bool done(true);

	for(size_t i(0); i < allPath_.size(); ++i){

		glm::vec3 const& pathPos(allPath_[i].getPosition());


		allDelta_[i] += toAdd * 0.01 * 1;

		if(allDelta_[i] > 1.0f){
			allDelta_[i] = 1.0f;
		}
		else if(allDelta_[i] < 0.0f){
			allDelta_[i] = 0.0f;
		}
		else{
			done = false;
		}

		float pos(-1.0f * (1.0f - allDelta_[i]) + i/30.0f * allDelta_[i]);

		allPath_[i].setPosition(pathPos.x, pos, pathPos.z);

	}

	if(done){
		inTransition_ = false;
	}


}


void GraphicEnvironement::draw() const {


	for(size_t i(0); i < allNode_.size(); ++i){

		allNode_[i].draw();

	}

	for(size_t i(0); i < allPath_.size(); ++i){

		allPath_[i].draw();

	}

	for(size_t i(0); i < allCars_.size(); ++i){

		allCars_[i].draw();

	}

}


void GraphicEnvironement::switchView(){

	explodedView_ = 1 - explodedView_;

	inTransition_ = true;

}



