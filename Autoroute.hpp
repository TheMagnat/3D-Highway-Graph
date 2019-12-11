#pragma once


#include <vector>
#include <map>

#include <thread>
#include <mutex>

class Autoroute{

public:

	struct Node{

		Node(size_t id, int x, int y);

		Node(Node const& copied);

		size_t id_;
		int x_;
		int y_;

		bool nearDone_;

		bool taken_;

		std::vector<Node*> nextNode_;


		std::mutex blocked_;

	};
	
	Autoroute();

	//Fonction qui dit si un noeud est présent en x et y
	bool find(int x, int y);

	//Genere tout les noeuds et les routes
	void generateRoads(size_t nbNode);

	//Gère les points de depart des voitures
	void startPoint(size_t nodeIndex);

	//Gère les points d'arrivé des voitures
	void stopPoint(size_t nodeIndex);

	//Fonction lancé par les voiture pour choisir leur prochain noeud
	size_t pickRandomNext(size_t actualNode, size_t carIndex);

	//Fonction de debug pour afficher les noeuds
	void print();

	//Fonction qui retourne le vecteur de noeuds
	std::vector<Autoroute::Node> const& getNodeInfo() const;
	
private:

	std::vector<Node> allNode_;

	std::map<int, std::map<int, size_t>> worldPos_;


};