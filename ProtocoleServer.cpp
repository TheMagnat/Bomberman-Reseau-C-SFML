#include "ProtocoleServer.hpp"

#include <iostream>

ProtocoleServer::ProtocoleServer(int *etat, size_t *hote) : startBomb_(1), startExplo_(1), startSpeed_(0), timeBomb_(2000), startVie_(1), chanceBonus_(40), chanceBombe_(1), chanceExplo_(1), chanceSpeed_(1), idBomb_(0), etat_(etat), hote_(hote) {
	head_.resize(3);
}

void ProtocoleServer::FillPlayers(std::string& AllData){
	players_.clear();
	int num, totalTaille;

	totalTaille = takeNextInt(AllData.data()+4, num);

	alivePlayers_ = num;

	for(int i(0); i < num; ++i){
		totalTaille += takeNextInt(AllData.data()+5+totalTaille+(i*3), tempoNum);
		totalTaille += takeNextInt(AllData.data()+6+totalTaille+(i*3), tempoX);
		totalTaille += takeNextInt(AllData.data()+7+totalTaille+(i*3), tempoY);
		players_.emplace(std::make_pair(tempoNum, infoPlayer(tempoX, tempoY, startBomb_, startExplo_, 0.4f - startSpeed_*0.033, startVie_)));

		map_[tempoX + tempoY * len_] = tempoNum + 4;
	}
}

void ProtocoleServer::FillPlayersFd(std::vector<pollfd> const& fds){
	AllPlayersFd_.clear();
	for(int i(1), nb(fds.size()); i < nb; ++i){
		AllPlayersFd_.push_back(fds[i].fd);
	}
}

void ProtocoleServer::FillMap(std::string& AllData){
	map_.clear();
	int totalTaille;

	totalTaille = takeNextInt(AllData.data()+4, tempoX);
	totalTaille += takeNextInt(AllData.data()+5+totalTaille, tempoY);

	len_ = tempoX;
	height_ = tempoY;
	
	for(int i(0), size(tempoX*tempoY); i < size; ++i){
		totalTaille += takeNextInt(AllData.data()+totalTaille+6+i, tempoNum);
		map_.emplace_back(tempoNum);
	}
}

void ProtocoleServer::FillCfg(){

	sendBuff = "CFG " + std::to_string(startBomb_) + " " + 
	std::to_string(startExplo_) + " " + std::to_string(startSpeed_) + " " + 
	std::to_string(timeBomb_) + " " + std::to_string(startVie_) + " " + 
	std::to_string(chanceBonus_) + " " + std::to_string(chanceBombe_) + " " + 
	std::to_string(chanceExplo_) + " " + std::to_string(chanceSpeed_) + "\n";

}

size_t ProtocoleServer::takeNextMessage(const char * buff, std::string& in){
	int taille;

	in.clear();
	for(taille = 0; buff[taille] != '\n' && buff[taille] != '\0'; ++taille){
		in.push_back(buff[taille]);
	}

	return taille;
}

size_t ProtocoleServer::takeNextWord(const char * buff, std::string& in){
	int taille;

	in.clear();
	for(taille = 0; buff[taille] != ' ' && buff[taille] != '\n' && buff[taille] != '\0'; ++taille){
		in.push_back(buff[taille]);
	}

	return taille;
}

size_t ProtocoleServer::takeNextInt(const char * buff, int& in){
	int taille;

	for(taille = 0; buff[taille] != ' ' && buff[taille] != '\n' && buff[taille] != '\0'; ++taille){
		tempoData[taille] = buff[taille];
	}
	tempoData[taille] = '\0';

	in = atoi(tempoData);
	return taille;
}

int ProtocoleServer::beforeGameRecv(char buff[BUFFER_SIZE], int num){
	head_[0] = buff[0];
	head_[1] = buff[1];
	head_[2] = buff[2];

	if(!head_.compare("LOG")){
		takeNextWord(buff+4, tempoBuff);
		sendBuff = "ARV " + std::to_string(num) + " " + tempoBuff + "\n";
		return 1;
	}
	else if(!head_.compare("MSG")){
		takeNextMessage(buff+4, tempoBuff);
		sendBuff = "MSG " + std::to_string(num) + " " + tempoBuff + "\n";
		return 2;
	}
	else if(!head_.compare("NXT")){
		if(static_cast<size_t>(num) != *hote_) return 0;
		return 3;
	}
	else if(!head_.compare("PRV")){
		if(static_cast<size_t>(num) != *hote_) return 0;
		return 4;
	}
	else if(!head_.compare("QUT")){
		sendBuff = "LFT " + std::to_string(num) + "\n";
		return 69;
	}
	else if(!head_.compare("RDY")){
		if(static_cast<size_t>(num) != *hote_) return 0;
		return 70;
	}
	else if(!head_.compare("SET")){
		if(static_cast<size_t>(num) != *hote_) return 0;
		int tempoBomb, tempoExplo, tempoSpeed, tempoTimeBomb, tempoVie;

		taille_ = takeNextInt(buff+4, tempoBomb);
		if(taille_ == 0) return 0;
		if(tempoBomb < 1) return 0;

		taille2_ = takeNextInt(buff+5+taille_, tempoExplo);
		if(taille2_ == 0) return 0;
		if(tempoExplo < 1) return 0;

		size_t taille3_ = takeNextInt(buff+6+taille_+taille2_, tempoSpeed);
		if(taille3_ == 0) return 0;
		if(tempoSpeed < 0 || tempoSpeed > 9) return 0;

		size_t taille4_ = takeNextInt(buff+7+taille_+taille2_+taille3_, tempoTimeBomb);
		if(taille4_ == 0) return 0;
		if(tempoTimeBomb < 0) return 0;

		size_t taille5_ = takeNextInt(buff+8+taille_+taille2_+taille3_+taille4_, tempoVie);
		if(taille5_ == 0) return 0;
		if(tempoVie < 1) return 0;	

		startBomb_ = tempoBomb;
		startExplo_ = tempoExplo;
		startSpeed_ = tempoSpeed;

		timeBomb_ = tempoTimeBomb;
		startVie_ = tempoVie;

		FillCfg();
		return 2;
	}

	else if(!head_.compare("CHA")){
		if(static_cast<size_t>(num) != *hote_) return 0;
		int tempoAll, tempoBomb, tempoExplo, tempoSpeed;

		taille_ = takeNextInt(buff+4, tempoAll);
		if(taille_ == 0) return 0;
		if(tempoAll < 0) return 0;

		taille2_ = takeNextInt(buff+5+taille_, tempoBomb);
		if(taille2_ == 0) return 0;
		if(tempoBomb < 0) return 0;

		size_t taille3_ = takeNextInt(buff+6+taille_+taille2_, tempoExplo);
		if(taille3_ == 0) return 0;
		if(tempoExplo < 0) return 0;

		size_t taille4_ = takeNextInt(buff+7+taille_+taille2_+taille3_, tempoSpeed);
		if(taille4_ == 0) return 0;
		if(tempoSpeed < 0) return 0;

		if((tempoBomb + tempoExplo + tempoSpeed) == 0) tempoAll = 0;

		chanceBonus_ = tempoAll;
		chanceBombe_ = tempoBomb;
		chanceExplo_ = tempoExplo;
		chanceSpeed_ = tempoSpeed;

		FillCfg();
		return 2;
	}

	return 0;
}

int ProtocoleServer::duringGameRecv(char buff[BUFFER_SIZE], int num){

	if(!players_.at(num).alive) return 0;

	head_[0] = buff[0];
	head_[1] = buff[1];
	head_[2] = buff[2];

	if(!head_.compare("MOV")){
		if(players_.at(num).temps.getElapsedTime().asSeconds() < players_.at(num).speed) return 0;

		int newX(players_.at(num).x), newY(players_.at(num).y);

		if(buff[4] == 'L'){
			--newX;
		}
		else if(buff[4] == 'R'){
			++newX;
		}
		else if(buff[4] == 'B'){
			++newY;
		}
		else if(buff[4] == 'T'){
			--newY;
		}

		if(newX >= static_cast<int>(len_) || newX < 0) return 0;

		int pos(newX+(newY*len_));
		if(pos < 0 || pos >= static_cast<int>(map_.size())) return 0;
		if(map_[pos] > 0 && map_[pos] < 4) return 0;

		if(map_[pos] < -3) getBonus(pos, num);

		int oldPos(players_.at(num).x + (players_.at(num).y * len_));

		if(map_[oldPos] > 3) map_[oldPos] = 0;

		map_[pos] = num + 4;
		players_.at(num).x = newX;
		players_.at(num).y = newY;
		sendBuff = "POS " + std::to_string(num) + " " + std::to_string(newX) + " " + std::to_string(newY) + "\n";
		players_.at(num).temps.restart();
		return 1;
	}
	else if(!head_.compare("BMB")){
		if(!players_.at(num).nbBomb) return 0;

		tempoX = players_.at(num).x;
		tempoY = players_.at(num).y;

		int pos(tempoX + (tempoY * len_));

		if(map_[pos] > 0 && map_[pos] < 4) return 0;

		--players_.at(num).nbBomb;

		sendBuff = "BMB " + std::to_string(tempoX) + " " + std::to_string(tempoY) + " " + std::to_string(players_.at(num).power) + "\n";
		
		map_[pos] = 3;
		bombId_[pos] = idBomb_;
		bombPosToOwner_[tempoX][tempoY] = num;



		std::thread newThread(&ProtocoleServer::waiterBomb, this, tempoX, tempoY, idBomb_++, num);
		newThread.detach();
		return 1;
	}

	return 0;
}

int ProtocoleServer::afterGameRecv(char buff[BUFFER_SIZE], int num){

	head_[0] = buff[0];
	head_[1] = buff[1];
	head_[2] = buff[2];

	if(!head_.compare("RST")){

		if(static_cast<size_t>(num) != *hote_) return 0;

		sendBuff = "RST\n";

		for(auto it(players_.begin()), itEnd(players_.end()); it != itEnd; ++it){
			it->second.alive = true;
		}

		return 1;
	}

	return 0;
}

void ProtocoleServer::bombBlow(int x, int y, int numPose, int depth, std::unordered_set<int>& toBlow_){

	int owner(bombPosToOwner_[x][y]);

	bombPosToOwner_[x].erase(y);

	bombId_.erase(x + y*len_);

	++players_.at(owner).nbBomb;

	int pow(players_.at(owner).power);

	std::string tempoSender("EXP " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(pow) + "\n");
	for(size_t i(0), size(AllPlayersFd_.size()); i < size; ++i){
		send(AllPlayersFd_[i], tempoSender.data(), tempoSender.size(), 0);
	}

	int initialPos(x + y * len_);
	int minY(y - pow), maxY(y + pow);
	int minX(x - pow), maxX(x + pow);

	map_[initialPos] = -3;

	if(minY < 0) minY = 0;
	if(maxY >= static_cast<int>(height_)) maxY = height_-1;

	if(minX < 0) minX = 0;
	if(maxX >= static_cast<int>(len_)) maxX = len_-1;

	minY = x + minY*len_;
	maxY = x + maxY*len_;
	minX = minX + y*len_;
	maxX = maxX + y*len_;

	for(int i(initialPos-1); i >= minX; --i){
		if(map_[i] == 0){
			continue;
		}
		else if(map_[i] == 1){
			toBlow_.insert(i);
			addBonus(i);
			break;
		}
		else if(map_[i] == 2 || map_[i] == -3){
			break;
		}
		else if(map_[i] == 3){
			bombBlow(i%len_, (i-i%len_)/len_, numPose, depth + 1, toBlow_);
			break;
		}
		else if(map_[i] > 3){
			playerKill(map_[i] - 4, numPose, i);
		}
	}

	for(int i(initialPos+1); i <= maxX; ++i){
		if(map_[i] == 0){
			continue;
		}
		else if(map_[i] == 1){
			toBlow_.insert(i);
			addBonus(i);
			break;
		}
		else if(map_[i] == 2 || map_[i] == -3){
			break;
		}
		else if(map_[i] == 3){
			bombBlow(i%len_, (i-i%len_)/len_, numPose, depth + 1, toBlow_);
			break;
		}
		else if(map_[i] > 3){
			playerKill(map_[i] - 4, numPose, i);
		}
	}

	for(int i(initialPos-len_); i >= minY; i -= len_){
		if(map_[i] == 0){
			continue;
		}
		else if(map_[i] == 1){
			toBlow_.insert(i);
			addBonus(i);
			break;
		}
		else if(map_[i] == 2 || map_[i] == -3){
			break;
		}
		else if(map_[i] == 3){
			bombBlow(i%len_, (i-i%len_)/len_, numPose, depth + 1, toBlow_);
			break;
		}
		else if(map_[i] > 3){
			playerKill(map_[i] - 4, numPose, i);
		}
	}

	for(int i(initialPos+len_); i <= maxY; i += len_){
		if(map_[i] == 0){
			continue;
		}
		else if(map_[i] == 1){
			toBlow_.insert(i);
			addBonus(i);
			break;
		}
		else if(map_[i] == 2 || map_[i] == -3){
			break;
		}
		else if(map_[i] == 3){
			bombBlow(i%len_, (i-i%len_)/len_, numPose, depth + 1, toBlow_);
			break;
		}
		else if(map_[i] > 3){
			playerKill(map_[i] - 4, numPose, i);
		}
	}

	//Pour vérifier si la bombe n'est pas sous un joueur 
	for(auto it(players_.begin()), itEnd(players_.end()); it != itEnd; ++it){
		if(it->second.alive){
			if(x == it->second.x && y == it->second.y) playerKill(it->first, numPose, initialPos);
		}
	}

	map_[initialPos] = 0;


	if(depth == 0){

		if(toBlow_.empty()){
			if(alivePlayers_ <= 1){
				endGame();
			}
			return;
		}

		//Header BRK
		std::string toSendBrk("BRK");


		for(auto it(toBlow_.begin()), itEnd(toBlow_.end()); it != itEnd; ++it){

			int blockIndex(*it);

			if(map_[blockIndex] > -4) map_[blockIndex] = 0;

			toSendBrk.push_back(' ');

			//indice X
			toSendBrk += std::to_string(blockIndex%len_);
			toSendBrk.push_back(' ');
			//indice Y
			toSendBrk += std::to_string((blockIndex-blockIndex%len_)/len_);

		}

		toSendBrk.push_back('\n');

		for(size_t i(0), size(AllPlayersFd_.size()); i < size; ++i){
			send(AllPlayersFd_[i], toSendBrk.data(), toSendBrk.size(), 0);
		}

		if(alivePlayers_ <= 1){
			endGame();
		}

	}

}

void ProtocoleServer::waiterBomb(int x, int y, int id, int numPose){
	
	std::this_thread::sleep_for(std::chrono::milliseconds(timeBomb_));

	if(map_[x + y * len_] != 3) return;
	if(bombId_.find(x + y*len_) == bombId_.end()) return;
	if(bombId_[x + y*len_] != id) return;

	std::unordered_set<int> toBlow;

	bombBlow(x, y, numPose, 0, toBlow);
}

void ProtocoleServer::addBonus(int index){

	if(rand()%100 >= chanceBonus_) return;

	int on(chanceBombe_ + chanceExplo_ + chanceSpeed_);

	int type(rand()%on);

	if(type < chanceBombe_){
		type = 0;
	}
	else if(type < chanceExplo_ + chanceBombe_){
		type = 1;
	}
	else{
		type = 2;
	}

	std::string sendBonus("BNS " + std::to_string(index%len_) + " " + std::to_string((index-index%len_)/len_) + " " + std::to_string(type) + "\n");
	for(size_t i(0), size(AllPlayersFd_.size()); i < size; ++i){
		send(AllPlayersFd_[i], sendBonus.data(), sendBonus.size(), 0);
	}

	map_[index] = -type - 4;
}

void ProtocoleServer::getBonus(int pos, int numPlayer){

	int bonusType(-map_[pos] - 4);

	if(bonusType == 0){
		++players_.at(numPlayer).nbBomb;
	}
	else if(bonusType == 1){
		++players_.at(numPlayer).power;
	}
	else if(bonusType == 2){
		players_.at(numPlayer).speed -= 0.05f;
		if(players_.at(numPlayer).speed < 0.1f) players_.at(numPlayer).speed = 0.1f;
	}

	std::string sendGot("GOT " + std::to_string(pos%len_) + " " + std::to_string((pos-pos%len_)/len_) + "\n");
	for(size_t i(0), size(AllPlayersFd_.size()); i < size; ++i){
		send(AllPlayersFd_[i], sendGot.data(), sendGot.size(), 0);
	}

}

void ProtocoleServer::playerKill(int num, int tueur, int posIndex){

	if(players_.at(num).invinci.getElapsedTime().asSeconds() < 1.0f) return;

	players_.at(num).invinci.restart();

	if(--players_.at(num).vie > 0) return;

	std::string tempoSender("DTH " + std::to_string(num) + " " + std::to_string(tueur) + "\n");
	for(size_t i(0), size(AllPlayersFd_.size()); i < size; ++i){
		send(AllPlayersFd_[i], tempoSender.data(), tempoSender.size(), 0);
	}

	players_.at(num).alive = false;

	--alivePlayers_;

	map_[posIndex] = 0;
}

void ProtocoleServer::disconnect(int num){
	players_.at(num).alive = false;
	--alivePlayers_;

	if(alivePlayers_ <= 1){
			endGame();
	}
}

void ProtocoleServer::endGame(){

	if(*etat_ != 1) return;

	*etat_ = 2;

	std::string endString("END ");

	if(alivePlayers_ == 0){
		endString += ("-1");
		std::cout << "Fin de partie, match nul\n";
	}
	else{

		for(auto it(players_.begin()), itEnd(players_.end()); it != itEnd; ++it){
			if(it->second.alive){
				endString += std::to_string(it->first);
				std::cout << "Fin de partie, remporté par client numéro " << it->first << std::endl;
				break;
			}
		}

	}

	endString.push_back('\n');

	for(size_t i(0), size(AllPlayersFd_.size()); i < size; ++i){
		send(AllPlayersFd_[i], endString.data(), endString.size(), 0);
	}

}