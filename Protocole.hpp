#pragma once

#include <string>
#include <cstdlib>
#include <vector>
#include "multiplayer.h"
#include "macro.h"
#include "OptionChoice.hpp"

struct playerInfo{
	playerInfo(int numm, int xx, int yy, std::string const& namee) : num(numm), x(xx), y(yy), name(namee) {}
	int num;
	int x;
	int y;
	std::string name;
};

class Protocole {
public:

	explicit Protocole(int sock);

	void fillAll(std::vector<std::pair<int, std::pair<int, int>>>& toFill);
	void fillLst(std::vector<std::pair<int, std::string>>& toFill);

	void fillOption(OptionChoice& toFillOption);

	int beforeGameRecv();
	int beforeGameRecv2();
	int duringGame();

	void sendMove(uint8_t x);

	void sendBomb();

	void sendRST(int mode);

private:

	size_t takeNextWord(char * buff, std::string& in);
	
	size_t takeNextInt(char * buff, int& in);

	int myRecvNoBlock();
	bool myRecv();

	bool nextRecv();

public:
	char buff_[BUFFER_SIZE];
	char tempoData[BUFFER_SIZE];

	std::string tempoBuff;
	int tempoNum;
	int tempoX;
	int tempoY;
	
	size_t tempoSize;

	int sock_;
private:
	bool fin_;

	ssize_t tailleTotal_;
	ssize_t taille_;
	ssize_t taille2_;
	size_t taille3_;
	std::string head_;
};