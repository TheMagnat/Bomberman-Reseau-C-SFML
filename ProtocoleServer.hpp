#pragma once

#include <string>
#include <cstdlib>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <SFML/System/Clock.hpp>
#include <unistd.h>
#include <thread>
#include <poll.h>
#include <sys/socket.h>
#include <chrono>


#include "macro.h"


struct infoPlayer{
	infoPlayer(int xx, int yy, int nbBombb, int powerr, float speedd, size_t viee) : x(xx), y(yy), nbBomb(nbBombb), power(powerr), speed(speedd), vie(viee), alive(true) {}
	int x;
	int y;
	int nbBomb;
	int power;

	float speed;
	sf::Clock temps;

	size_t vie;
	sf::Clock invinci;

	bool alive;
};

class ProtocoleServer {
public:

	explicit ProtocoleServer(int *etat, size_t *hote);

	void FillPlayers(std::string& AllData);
	void FillPlayersFd(std::vector<pollfd> const& fds);
	void FillMap(std::string& AllData);
	void FillCfg();

	size_t takeNextMessage(const char * buff, std::string& in);

	size_t takeNextWord(const char * buff, std::string& in);
	
	size_t takeNextInt(const char * buff, int& in);

	int beforeGameRecv(char buff[BUFFER_SIZE], int num);
	int duringGameRecv(char buff[BUFFER_SIZE], int num);
	int afterGameRecv(char buff[BUFFER_SIZE], int num);


	void bombBlow(int x, int y, int numPose, int depth, std::unordered_set<int>& toBlow_);
	void waiterBomb(int x, int y, int id, int numPose);

	void addBonus(int index);
	void getBonus(int pos, int numPlayer);

	void playerKill(int num, int tueur, int posIndex);
	void disconnect(int num);

	void endGame();

public:
	char buff_[BUFFER_SIZE];
	char tempoData[BUFFER_SIZE];

	std::string sendBuff;
	std::string tempoBuff;
	int tempoNum;
	int tempoX;
	int tempoY;

private:
	//Paramettre de partie
	int startBomb_;
	int startExplo_;
	int startSpeed_;
	size_t timeBomb_;
	size_t startVie_;

	//Autre paremetres sur la chance
	int chanceBonus_;
	int chanceBombe_;
	int chanceExplo_;
	int chanceSpeed_;

	ssize_t taille_;
	size_t taille2_;
	std::string head_;

	size_t alivePlayers_;
	std::unordered_map<int, infoPlayer> players_;
	
	//Permet de d'attribuer un identidiant à une bombe pour qu'un thread qui se reveille soit sûr que la bombe se trouvant sur sa case soit bien la même qu'a son lancement
	std::unordered_map<int, int> bombId_;
	int idBomb_;

	std::unordered_map<int, std::unordered_map<int, int>> bombPosToOwner_;

	//Pour envoyer des messages indépendament, utiles pour les thread de bombes.
	std::vector<int> AllPlayersFd_;

	std::vector<int> map_;
	size_t len_;
	size_t height_;

	//Sauvegarde de l'etat de la partie et de l'hote
	int *etat_;
	size_t *hote_;
};