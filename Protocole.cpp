#include "Protocole.hpp"

#include <iostream>

Protocole::Protocole(int sock) : sock_(sock), fin_(false) {
	head_.resize(3);
}

void Protocole::fillAll(std::vector<std::pair<int, std::pair<int, int>>>& toFill){
	int num, totalTaille;

	totalTaille = takeNextInt(buff_+4, num);
	for(int i(0); i < num; ++i){
		totalTaille += takeNextInt(buff_+5+totalTaille+(i*3), tempoNum);
		totalTaille += takeNextInt(buff_+6+totalTaille+(i*3), tempoX);
		totalTaille += takeNextInt(buff_+7+totalTaille+(i*3), tempoY);

		toFill.emplace_back(std::make_pair(tempoNum, std::make_pair(tempoX, tempoY)));
	}
}

void Protocole::fillLst(std::vector<std::pair<int, std::string>>& toFill){
	int num, totalTaille;

	totalTaille = takeNextInt(buff_+4, num);

	for(int i(0); i < num; ++i){
		totalTaille += takeNextInt(buff_+5+totalTaille+(i*2), tempoNum);
		totalTaille += takeNextWord(buff_+6+totalTaille+(i*2), tempoBuff);

		toFill.emplace_back(std::make_pair(tempoNum, tempoBuff));
	}
}

void Protocole::fillOption(OptionChoice& toFillOption){

	std::array<int, 9> cfgOption;
	size_t totalTaille(0);

	for(size_t i(0); i < 9; ++i){
		int tempoInt;
		totalTaille += takeNextInt(buff_+4+totalTaille+i, tempoInt);
		cfgOption[i] = tempoInt;
	}

	toFillOption.setAll(cfgOption);

}

size_t Protocole::takeNextWord(char * buff, std::string& in){
	int taille;

	in.clear();
	for(taille = 0; buff[taille] != ' ' && buff[taille] != '\n' && buff[taille] != '\0'; ++taille){
		in.push_back(buff[taille]);
	}

	return taille;
}

size_t Protocole::takeNextInt(char * buff, int& in){
	int taille;

	for(taille = 0; buff[taille] != ' ' && buff[taille] != '\n' && buff[taille] != '\0'; ++taille){
		tempoData[taille] = buff[taille];
	}
	tempoData[taille] = '\0';

	in = atoi(tempoData);
	return taille;
}

bool Protocole::nextRecv(){
	ssize_t i;


	for(i = taille_; i < tailleTotal_; ++i){
		buff_[i-taille_] = buff_[i];
	}
	tailleTotal_ -= taille_;

	for(taille_ = 0; taille_ < tailleTotal_; ++taille_){
		if(buff_[taille_] == '\n' || buff_[taille_] == '\0' || buff_[taille_] == '\r'){
			break;
		}
	}

	for(; taille_ < tailleTotal_; ++taille_){
		if(buff_[taille_] != '\n' && buff_[taille_] != '\0' && buff_[taille_] != '\r'){
			break;
		}
	}

	if(taille_ == tailleTotal_){
		return false;
	}
	else{
		return true;
	}
}

int Protocole::myRecvNoBlock(){
	tailleTotal_ = recv(sock_, buff_, sizeof(buff_), MSG_DONTWAIT);
	if(tailleTotal_ < 0) return -1;
	if(tailleTotal_ == 0) return -2;

	for(taille_ = 0; taille_ < tailleTotal_; ++taille_){
		if(buff_[taille_] == '\n' || buff_[taille_] == '\0' || buff_[taille_] == '\r'){
			break;
		}
	}

	for(; taille_ < tailleTotal_; ++taille_){
		if(buff_[taille_] != '\n' && buff_[taille_] != '\0' && buff_[taille_] != '\r'){
			break;
		}
	}


	if(taille_ == tailleTotal_){
		return 0;
	}
	else{
		return 1;
	}
}

bool Protocole::myRecv(){
	tailleTotal_ = recv(sock_, buff_, sizeof(buff_), 0);

	for(taille_ = 0; taille_ < tailleTotal_; ++taille_){
		if(buff_[taille_] == '\n' || buff_[taille_] == '\0' || buff_[taille_] == '\r'){
			break;
		}
	}

	for(; taille_ < tailleTotal_; ++taille_){
		if(buff_[taille_] != '\n' && buff_[taille_] != '\0' && buff_[taille_] != '\r'){
			break;
		}
	}


	if(taille_ == tailleTotal_){
		return false;
	}
	else{
		return true;
	}
}

int Protocole::beforeGameRecv(){
	if(fin_){
		fin_ = nextRecv();
	}
	else{
		int save = myRecvNoBlock();
		if(save == -1) return 0;
		if(save == -2) return -1;
		if(save == 1) fin_ = true;
	}
	head_[0] = buff_[0];
	head_[1] = buff_[1];
	head_[2] = buff_[2];


	if(!head_.compare("ARV")){
		taille3_ = takeNextInt(buff_+4, tempoNum);
		takeNextWord(buff_+5+taille3_, tempoBuff);
		return 1;
	}
	else if(!head_.compare("LFT")){
		takeNextInt(buff_+4, tempoNum);
		return 2;
	}
	else if(!head_.compare("CND")){
		takeNextInt(buff_+4, tempoNum);
		return 3;
	}
	else if(!head_.compare("LST")){
		return 4;
	}
	else if(!head_.compare("MSG")){
		tempoSize = takeNextInt(buff_+4, tempoNum);
		buff_[taille_-1] = '\0';
		return 5;
	}
	else if(!head_.compare("ACT")){
		taille3_ = takeNextInt(buff_+4, tempoNum);
		takeNextWord(buff_+5+taille3_, tempoBuff);
		return 6;
	}
	else if(!head_.compare("CFG")){
		return 7;
	}
	else if(!head_.compare("MAP")){
		taille3_ = takeNextInt(buff_+4, tempoX);
		taille2_ = takeNextInt(buff_+5+taille3_, tempoY);
		tempoNum = 6+taille3_+taille2_;
		return 42;
	}

	return 12;
}

int Protocole::beforeGameRecv2(){
	if(fin_){
		fin_ = nextRecv();
	}
	else{
		int save = myRecvNoBlock();
		if(save == -1) return 0;
		if(save == -2) return -1;
		if(save == 1) fin_ = true;
	}
	head_[0] = buff_[0];
	head_[1] = buff_[1];
	head_[2] = buff_[2];


	if(!head_.compare("ALL")){
		return 1;
	}
	else if(!head_.compare("STR")){
		return 42;
	}

	return 12;
}

//POS NUM X Y
int Protocole::duringGame(){

	if(fin_){
		fin_ = nextRecv();
	}
	else{
		int save = myRecvNoBlock();
		if(save == -1) return 0;
		if(save == -2) return -1;
		if(save == 1) fin_ = true;

	}

	head_[0] = buff_[0];
	head_[1] = buff_[1];
	head_[2] = buff_[2];

	if(!head_.compare("POS")){
		//On retire les espace et le header du buffer a chaque fois
		taille3_ = takeNextInt(buff_+4, tempoNum);
		taille2_ = takeNextInt(buff_+5+taille3_, tempoX);
		takeNextInt(buff_+6+taille3_+taille2_, tempoY);
		return 1;
	}
	else if(!head_.compare("BMB")){
		//On retire les espace et le header du buffer a chaque fois
		taille3_ = takeNextInt(buff_+4, tempoX);
		taille2_ = takeNextInt(buff_+5+taille3_, tempoY);
		takeNextInt(buff_+6+taille3_+taille2_, tempoNum);
		return 2;
	}
	else if(!head_.compare("EXP")){
		//On retire les espace et le header du buffer a chaque fois
		taille3_ = takeNextInt(buff_+4, tempoX);
		taille2_ = takeNextInt(buff_+5+taille3_, tempoY);
		takeNextInt(buff_+6+taille3_+taille2_, tempoNum);
		return 3;
	}
	else if(!head_.compare("BRK")){
		buff_[taille_-1] = '\0';
		return 4;
	}
	else if(!head_.compare("BNS")){
		taille3_ = takeNextInt(buff_+4, tempoX); //X
		taille2_ = takeNextInt(buff_+5+taille3_, tempoY); //Y
		takeNextInt(buff_+6+taille3_+taille2_, tempoNum); //Type
		return 5;
	}
	else if(!head_.compare("GOT")){
		taille3_ = takeNextInt(buff_+4, tempoX);
		taille2_ = takeNextInt(buff_+5+taille3_, tempoY);
		return 6;
	}
	else if(!head_.compare("DTH")){
		//On retire les espace et le header du buffer a chaque fois
		taille3_ = takeNextInt(buff_+4, tempoX);
		taille2_ = takeNextInt(buff_+5+taille3_, tempoY);
		return 7;
	}
	else if(!head_.compare("END")){
		//On retire les espace et le header du buffer a chaque fois
		taille3_ = takeNextInt(buff_+4, tempoNum);
		return 8;
	}
	else if(!head_.compare("RST")){
		//On retire les espace et le header du buffer a chaque fois
		return 9;
	}
	else if(!head_.compare("LFT")){
		takeNextInt(buff_+4, tempoNum);
		return 10;
	}

	return 12;
}

void Protocole::sendMove(uint8_t x){
	if(x == 1){
		send(sock_, "MOV T\n", 6, 0);
	}
	else if(x == 2){
		send(sock_, "MOV B\n", 6, 0);
	}
	else if(x == 3){
		send(sock_, "MOV L\n", 6, 0);
	}
	else{
		send(sock_, "MOV R\n", 6, 0);
	}
}

void Protocole::sendBomb(){
	send(sock_, "BMB\n", 4, 0);
}

void Protocole::sendRST(int mode){
	std::string stringRST("RST\n");
	send(sock_, stringRST.data(), stringRST.size(), 0);
}