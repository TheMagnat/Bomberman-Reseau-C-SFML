#include "Carte.hpp"

Carte::Carte(std::string const& fichierTexture, std::string const& fichierTextureBomb) : longueur_(0), hauteur_(0), smokes_() {
	if(!texture_.loadFromFile(fichierTexture)){
		std::cerr << "Impossible d'ouvrir le fichier : " << fichierTexture << std::endl;
	}
	if(!textureBomb_.loadFromFile(fichierTextureBomb)){
		std::cerr << "Impossible d'ouvrir le fichier : " << fichierTextureBomb << std::endl;
	}
	if(!textureSmoke_.loadFromFile("ressources/sprites/smoke.png")){
		std::cerr << "Impossible d'ouvrir le fichier : " << "ressources/smoke.png" << std::endl;
	}
	if(!textureBonus_.loadFromFile("ressources/sprites/bonus.png")){
		std::cerr << "Impossible d'ouvrir le fichier : " << "ressources/sprites/bonus.png" << std::endl;
	}
	mur_.setPrimitiveType(sf::PrimitiveType::Quads);
	sol_.setPrimitiveType(sf::PrimitiveType::Quads);
}


Carte::Carte(std::string const& nomFichier, std::string const& fichierTexture, std::string const& fichierTextureBomb) {
	
	if(!texture_.loadFromFile(fichierTexture)){
		std::cerr << "Impossible d'ouvrir le fichier : " << fichierTexture << std::endl;
	}
	if(!textureBomb_.loadFromFile(fichierTextureBomb)){
		std::cerr << "Impossible d'ouvrir le fichier : " << fichierTexture << std::endl;
	}

	mur_.setPrimitiveType(sf::PrimitiveType::Quads);
	sol_.setPrimitiveType(sf::PrimitiveType::Quads);

	size_t i, j;
	int save;
	sf::Vertex tempo;
	sf::Vector2f pos;

	sf::IntRect textuPosBlock(160, 96, TILE_SIZE, TILE_SIZE);

	sf::Vector2f textuPos1(352, 32);
	sf::Vector2f textuPos2(352 + TILE_SIZE, 32);
	sf::Vector2f textuPos3(352 + TILE_SIZE, 32 + TILE_SIZE);
	sf::Vector2f textuPos4(352, 32 + TILE_SIZE);

	std::ifstream fichier(nomFichier, std::ios::in);
	fichier >> longueur_ >> hauteur_;

	
	//On dessine le sol et les bords de la map
	for(i=0; i<longueur_+2; ++i){
		tempo.position = sf::Vector2f(i*TILE_SIZE, 0);
		tempo.texCoords = sf::Vector2f(320, 0);
		sol_.append(tempo);

		tempo.position = sf::Vector2f(i*TILE_SIZE + TILE_SIZE, 0);
		tempo.texCoords = sf::Vector2f(320 + TILE_SIZE, 0);
		sol_.append(tempo);

		tempo.position = sf::Vector2f(i*TILE_SIZE + TILE_SIZE, TILE_SIZE);
		tempo.texCoords = sf::Vector2f(320 + TILE_SIZE, 0 + TILE_SIZE);
		sol_.append(tempo);

		tempo.position = sf::Vector2f(i*TILE_SIZE, TILE_SIZE);
		tempo.texCoords = sf::Vector2f(320, 0 + TILE_SIZE);
		sol_.append(tempo);
	}

	for(j=1; j<hauteur_+1; ++j){

		tempo.position = sf::Vector2f(0, j*TILE_SIZE);
		tempo.texCoords = sf::Vector2f(320, 0);
		sol_.append(tempo);

		tempo.position = sf::Vector2f(TILE_SIZE, j*TILE_SIZE);
		tempo.texCoords = sf::Vector2f(320 + TILE_SIZE, 0);
		sol_.append(tempo);

		tempo.position = sf::Vector2f(TILE_SIZE, j*TILE_SIZE + TILE_SIZE);
		tempo.texCoords = sf::Vector2f(320 + TILE_SIZE, 0 + TILE_SIZE);
		sol_.append(tempo);

		tempo.position = sf::Vector2f(0, j*TILE_SIZE + TILE_SIZE);
		tempo.texCoords = sf::Vector2f(320, 0 + TILE_SIZE);
		sol_.append(tempo);

		for(i=1; i<longueur_+1; ++i){

			tempo.position = sf::Vector2f(i*TILE_SIZE, j*TILE_SIZE);
			tempo.texCoords = sf::Vector2f(0, 0);
			sol_.append(tempo);

			tempo.position = sf::Vector2f(i*TILE_SIZE + TILE_SIZE, j*TILE_SIZE);
			tempo.texCoords = sf::Vector2f(0 + TILE_SIZE, 0);
			sol_.append(tempo);

			tempo.position = sf::Vector2f(i*TILE_SIZE + TILE_SIZE, j*TILE_SIZE + TILE_SIZE);
			tempo.texCoords = sf::Vector2f(0 + TILE_SIZE, 0 + TILE_SIZE);
			sol_.append(tempo);

			tempo.position = sf::Vector2f(i*TILE_SIZE, j*TILE_SIZE + TILE_SIZE);
			tempo.texCoords = sf::Vector2f(0, 0 + TILE_SIZE);
			sol_.append(tempo);

		}

		tempo.position = sf::Vector2f(i*TILE_SIZE, j*TILE_SIZE);
		tempo.texCoords = sf::Vector2f(320, 0);
		sol_.append(tempo);

		tempo.position = sf::Vector2f(i*TILE_SIZE + TILE_SIZE, j*TILE_SIZE);
		tempo.texCoords = sf::Vector2f(320 + TILE_SIZE, 0);
		sol_.append(tempo);

		tempo.position = sf::Vector2f(i*TILE_SIZE + TILE_SIZE, j*TILE_SIZE + TILE_SIZE);
		tempo.texCoords = sf::Vector2f(320 + TILE_SIZE, 0 + TILE_SIZE);
		sol_.append(tempo);

		tempo.position = sf::Vector2f(i*TILE_SIZE, j*TILE_SIZE + TILE_SIZE);
		tempo.texCoords = sf::Vector2f(320, 0 + TILE_SIZE);
		sol_.append(tempo);

	}


	for(i=0; i<longueur_+2; ++i){
		tempo.position = sf::Vector2f(i*TILE_SIZE, j*TILE_SIZE);
		tempo.texCoords = sf::Vector2f(320, 0);
		sol_.append(tempo);

		tempo.position = sf::Vector2f(i*TILE_SIZE + TILE_SIZE, j*TILE_SIZE);
		tempo.texCoords = sf::Vector2f(320 + TILE_SIZE, 0);
		sol_.append(tempo);

		tempo.position = sf::Vector2f(i*TILE_SIZE + TILE_SIZE, j*TILE_SIZE + TILE_SIZE);
		tempo.texCoords = sf::Vector2f(320 + TILE_SIZE, 0 + TILE_SIZE);
		sol_.append(tempo);

		tempo.position = sf::Vector2f(i*TILE_SIZE, j*TILE_SIZE + TILE_SIZE);
		tempo.texCoords = sf::Vector2f(320, 0 + TILE_SIZE);
		sol_.append(tempo);
	}

	//On s'occupe maintenant des bloques dur et destructible
	for(i=1; i<hauteur_+1; ++i){
		for(j=1; j<longueur_+1; ++j){
			fichier >> save;
			data_.push_back(save);
			if(save){

				pos.x = j * TILE_SIZE;
				pos.y = i * TILE_SIZE;

				if(save == 1){
					blocks_[j-1].emplace(std::make_pair(i-1, Block(&texture_, pos, textuPosBlock)));
				}
				else if(save == 2){
					//Angle haut droit
					tempo.position = pos;
					tempo.texCoords = textuPos1;
					mur_.append(tempo);

					//Angle haut gauche
					tempo.position = sf::Vector2f(pos.x + TILE_SIZE, pos.y);
					tempo.texCoords = textuPos2;
					mur_.append(tempo);

					//Angle bas droit
					tempo.position = sf::Vector2f(pos.x + TILE_SIZE, pos.y + TILE_SIZE);
					tempo.texCoords = textuPos3;
					mur_.append(tempo);

					//Angle bas gauche
					tempo.position = sf::Vector2f(pos.x, pos.y + TILE_SIZE);
					tempo.texCoords = textuPos4;
					mur_.append(tempo);
				}
			}
		}
	}
	fichier.close();
}


void Carte::load(const char* data){

	size_t i, j;
	int save;
	sf::Vertex tempo;
	sf::Vector2f pos;

	sf::IntRect textuPosBlock(160, 96, TILE_SIZE, TILE_SIZE);

	sf::Vector2f textuPos1(352, 32);
	sf::Vector2f textuPos2(352 + TILE_SIZE, 32);
	sf::Vector2f textuPos3(352 + TILE_SIZE, 32 + TILE_SIZE);
	sf::Vector2f textuPos4(352, 32 + TILE_SIZE);

	StringToVec vecData(data);

	longueur_ = vecData.takeElement();
	hauteur_ = vecData.takeElement();

	//On dessine le sol et les bords de la map
	for(i=0; i<longueur_+2; ++i){
		tempo.position = sf::Vector2f(i*TILE_SIZE, 0);
		tempo.texCoords = sf::Vector2f(320, 0);
		sol_.append(tempo);

		tempo.position = sf::Vector2f(i*TILE_SIZE + TILE_SIZE, 0);
		tempo.texCoords = sf::Vector2f(320 + TILE_SIZE, 0);
		sol_.append(tempo);

		tempo.position = sf::Vector2f(i*TILE_SIZE + TILE_SIZE, TILE_SIZE);
		tempo.texCoords = sf::Vector2f(320 + TILE_SIZE, 0 + TILE_SIZE);
		sol_.append(tempo);

		tempo.position = sf::Vector2f(i*TILE_SIZE, TILE_SIZE);
		tempo.texCoords = sf::Vector2f(320, 0 + TILE_SIZE);
		sol_.append(tempo);
	}

	for(j=1; j<hauteur_+1; ++j){

		tempo.position = sf::Vector2f(0, j*TILE_SIZE);
		tempo.texCoords = sf::Vector2f(320, 0);
		sol_.append(tempo);

		tempo.position = sf::Vector2f(TILE_SIZE, j*TILE_SIZE);
		tempo.texCoords = sf::Vector2f(320 + TILE_SIZE, 0);
		sol_.append(tempo);

		tempo.position = sf::Vector2f(TILE_SIZE, j*TILE_SIZE + TILE_SIZE);
		tempo.texCoords = sf::Vector2f(320 + TILE_SIZE, 0 + TILE_SIZE);
		sol_.append(tempo);

		tempo.position = sf::Vector2f(0, j*TILE_SIZE + TILE_SIZE);
		tempo.texCoords = sf::Vector2f(320, 0 + TILE_SIZE);
		sol_.append(tempo);

		for(i=1; i<longueur_+1; ++i){

			tempo.position = sf::Vector2f(i*TILE_SIZE, j*TILE_SIZE);
			tempo.texCoords = sf::Vector2f(0, 0);
			sol_.append(tempo);

			tempo.position = sf::Vector2f(i*TILE_SIZE + TILE_SIZE, j*TILE_SIZE);
			tempo.texCoords = sf::Vector2f(0 + TILE_SIZE, 0);
			sol_.append(tempo);

			tempo.position = sf::Vector2f(i*TILE_SIZE + TILE_SIZE, j*TILE_SIZE + TILE_SIZE);
			tempo.texCoords = sf::Vector2f(0 + TILE_SIZE, 0 + TILE_SIZE);
			sol_.append(tempo);

			tempo.position = sf::Vector2f(i*TILE_SIZE, j*TILE_SIZE + TILE_SIZE);
			tempo.texCoords = sf::Vector2f(0, 0 + TILE_SIZE);
			sol_.append(tempo);

		}

		tempo.position = sf::Vector2f(i*TILE_SIZE, j*TILE_SIZE);
		tempo.texCoords = sf::Vector2f(320, 0);
		sol_.append(tempo);

		tempo.position = sf::Vector2f(i*TILE_SIZE + TILE_SIZE, j*TILE_SIZE);
		tempo.texCoords = sf::Vector2f(320 + TILE_SIZE, 0);
		sol_.append(tempo);

		tempo.position = sf::Vector2f(i*TILE_SIZE + TILE_SIZE, j*TILE_SIZE + TILE_SIZE);
		tempo.texCoords = sf::Vector2f(320 + TILE_SIZE, 0 + TILE_SIZE);
		sol_.append(tempo);

		tempo.position = sf::Vector2f(i*TILE_SIZE, j*TILE_SIZE + TILE_SIZE);
		tempo.texCoords = sf::Vector2f(320, 0 + TILE_SIZE);
		sol_.append(tempo);

	}


	for(i=0; i<longueur_+2; ++i){
		tempo.position = sf::Vector2f(i*TILE_SIZE, j*TILE_SIZE);
		tempo.texCoords = sf::Vector2f(320, 0);
		sol_.append(tempo);

		tempo.position = sf::Vector2f(i*TILE_SIZE + TILE_SIZE, j*TILE_SIZE);
		tempo.texCoords = sf::Vector2f(320 + TILE_SIZE, 0);
		sol_.append(tempo);

		tempo.position = sf::Vector2f(i*TILE_SIZE + TILE_SIZE, j*TILE_SIZE + TILE_SIZE);
		tempo.texCoords = sf::Vector2f(320 + TILE_SIZE, 0 + TILE_SIZE);
		sol_.append(tempo);

		tempo.position = sf::Vector2f(i*TILE_SIZE, j*TILE_SIZE + TILE_SIZE);
		tempo.texCoords = sf::Vector2f(320, 0 + TILE_SIZE);
		sol_.append(tempo);
	}


	//On s'occupe maintenant des bloques dur et destructible
	for(i=1; i<hauteur_+1; ++i){
		for(j=1; j<longueur_+1; ++j){
			save = vecData.takeElement();
			data_.push_back(save);
			if(save){

				pos.x = j * TILE_SIZE;
				pos.y = i * TILE_SIZE;

				if(save == 1){
					blocks_[j-1].emplace(std::make_pair(i-1, Block(&texture_, pos, textuPosBlock)));
				}
				else if(save == 2){
					//Angle haut droit
					tempo.position = pos;
					tempo.texCoords = textuPos1;
					mur_.append(tempo);

					//Angle haut gauche
					tempo.position = sf::Vector2f(pos.x + TILE_SIZE, pos.y);
					tempo.texCoords = textuPos2;
					mur_.append(tempo);

					//Angle bas droit
					tempo.position = sf::Vector2f(pos.x + TILE_SIZE, pos.y + TILE_SIZE);
					tempo.texCoords = textuPos3;
					mur_.append(tempo);

					//Angle bas gauche
					tempo.position = sf::Vector2f(pos.x, pos.y + TILE_SIZE);
					tempo.texCoords = textuPos4;
					mur_.append(tempo);
				}
			}
		}
	}
}

void Carte::breakWall(const char * data){

	StringToVec vecData(data);

	for(size_t i(0), taille(vecData.size_); i < taille; i += 2){

		int tempoX(vecData.takeElement());
		int tempoY(vecData.takeElement());

		data_[tempoX + tempoY * longueur_] = 0;
		blocks_[tempoX].erase(tempoY);

	}

}

void Carte::cleanSmoke(std::list<sf::VertexArray>::iterator it){
	std::this_thread::sleep_for(std::chrono::milliseconds(600));
	smokeDraw.lock();
	smokes_.erase(it);
	smokeDraw.unlock();
}

std::list<sf::VertexArray>::iterator Carte::addSmoke(int x, int minX, int maxX, int y, int minY, int maxY){
	sf::Vertex tempo;

	sf::Vector2f textuPos1(0, 0);
	sf::Vector2f textuPos2(TILE_SIZE, 0);
	sf::Vector2f textuPos3(TILE_SIZE, TILE_SIZE);
	sf::Vector2f textuPos4(0, TILE_SIZE);

	smokes_.emplace_front();
	auto ret = smokes_.begin();
	sf::VertexArray& tempoArray = smokes_.front();

	tempoArray.setPrimitiveType(sf::PrimitiveType::Quads);

	for(; minX <= maxX; ++minX){
		tempo.position = sf::Vector2f((minX+1)*TILE_SIZE, (y+1)*TILE_SIZE);
		tempo.texCoords = textuPos1;
		tempoArray.append(tempo);

		tempo.position = sf::Vector2f((minX+1)*TILE_SIZE + TILE_SIZE, (y+1)*TILE_SIZE);
		tempo.texCoords = textuPos2;
		tempoArray.append(tempo);

		tempo.position = sf::Vector2f((minX+1)*TILE_SIZE + TILE_SIZE, (y+1)*TILE_SIZE + TILE_SIZE);
		tempo.texCoords = textuPos3;
		tempoArray.append(tempo);

		tempo.position = sf::Vector2f((minX+1)*TILE_SIZE, (y+1)*TILE_SIZE + TILE_SIZE);
		tempo.texCoords = textuPos4;
		tempoArray.append(tempo);
	}
	for(; minY <= maxY; ++minY){
		tempo.position = sf::Vector2f((x+1)*TILE_SIZE, (minY+1)*TILE_SIZE);
		tempo.texCoords = textuPos1;
		tempoArray.append(tempo);

		tempo.position = sf::Vector2f((x+1)*TILE_SIZE + TILE_SIZE, (minY+1)*TILE_SIZE);
		tempo.texCoords = textuPos2;
		tempoArray.append(tempo);

		tempo.position = sf::Vector2f((x+1)*TILE_SIZE + TILE_SIZE, (minY+1)*TILE_SIZE + TILE_SIZE);
		tempo.texCoords = textuPos3;
		tempoArray.append(tempo);

		tempo.position = sf::Vector2f((x+1)*TILE_SIZE, (minY+1)*TILE_SIZE + TILE_SIZE);
		tempo.texCoords = textuPos4;
		tempoArray.append(tempo);
	}

	return ret;
}

void Carte::addaptMap(sf::RenderWindow& app){

	sf::Vector2u ecran(app.getSize());

	float tempoW((longueur_+2)*TILE_SIZE/(float)ecran.x), tempoH((hauteur_+2)*TILE_SIZE/(float)ecran.y);

	if(tempoW > tempoH){
		app.setView(sf::View(sf::Vector2f((longueur_+2)*TILE_SIZE/2, (hauteur_+2)*TILE_SIZE/2), sf::Vector2f((longueur_+2)*TILE_SIZE, ((longueur_+2)*TILE_SIZE)*((float)ecran.y/(float)ecran.x))));
	}
	else{
		app.setView(sf::View(sf::Vector2f((longueur_+2)*TILE_SIZE/2, (hauteur_+2)*TILE_SIZE/2), sf::Vector2f((hauteur_+2)*TILE_SIZE*((float)ecran.x/(float)ecran.y), (hauteur_+2)*TILE_SIZE)));
	}
}

void Carte::addBomb(int x, int y, int pow){
	bombs_[x].emplace(std::make_pair(y, Bomb(&textureBomb_, x, y, pow)));
	data_[x + y * longueur_] = 3;
}

void Carte::deleteBomb(int x, int y){ 
	if(bombs_[x].find(y) == bombs_[x].end()) return;

	SoundHandler::playBomb();

	int pow = bombs_[x].at(y).getPow();
	bombs_[x].erase(y);
	if(bombs_[x].empty()) bombs_.erase(x);

	int initialPos(x + y * longueur_);
	int minY(y - pow), maxY(y + pow);
	int minX(x - pow), maxX(x + pow);

	data_[initialPos] = -3;

	if(minY < 0) minY = 0;
	if(maxY >= static_cast<int>(hauteur_)) maxY = hauteur_-1;

	if(minX < 0) minX = 0;
	if(maxX >= static_cast<int>(longueur_)) maxX = longueur_-1;

	//auto it = addSmoke(x, minX, maxX, y, minY, maxY);

	sf::Vertex tempo;

	sf::Vector2f textuPos1(0, 0);
	sf::Vector2f textuPos2(TILE_SIZE, 0);
	sf::Vector2f textuPos3(TILE_SIZE, TILE_SIZE);
	sf::Vector2f textuPos4(0, TILE_SIZE);

	smokes_.emplace_front();
	auto tempoDeb = smokes_.begin();
	sf::VertexArray& tempoArray = smokes_.front();

	tempoArray.setPrimitiveType(sf::PrimitiveType::Quads);

	/*std::thread cleaner(&Carte::cleanSmoke, this, it);
	cleaner.detach();*/

	minY = x + minY*longueur_;
	maxY = x + maxY*longueur_;
	minX = minX + y*longueur_;
	maxX = maxX + y*longueur_;


	tempo.position = sf::Vector2f((x+1)*TILE_SIZE, (y+1)*TILE_SIZE);
	tempo.texCoords = textuPos1;
	tempoArray.append(tempo);

	tempo.position = sf::Vector2f((x+1)*TILE_SIZE + TILE_SIZE, (y+1)*TILE_SIZE);
	tempo.texCoords = textuPos2;
	tempoArray.append(tempo);

	tempo.position = sf::Vector2f((x+1)*TILE_SIZE + TILE_SIZE, (y+1)*TILE_SIZE + TILE_SIZE);
	tempo.texCoords = textuPos3;
	tempoArray.append(tempo);

	tempo.position = sf::Vector2f((x+1)*TILE_SIZE, (y+1)*TILE_SIZE + TILE_SIZE);
	tempo.texCoords = textuPos4;
	tempoArray.append(tempo);

	bool fin(false);
	for(int i(initialPos-1); i >= minX; --i){

		if(data_[i] == 1){
			//data_[i] = 0;
			//blocks_[i%longueur_].erase((i-i%longueur_)/longueur_);
			fin = true;
		}
		else if(data_[i] == 2 || data_[i] == -3){
			break;
		}
		else if(data_[i] == 3){
			deleteBomb(i%longueur_, (i-i%longueur_)/longueur_);
			break;
		}

		tempo.position = sf::Vector2f((i%longueur_+1)*TILE_SIZE, ((i-i%longueur_)/longueur_ + 1)*TILE_SIZE);
		tempo.texCoords = textuPos1;
		tempoArray.append(tempo);

		tempo.position = sf::Vector2f((i%longueur_+1)*TILE_SIZE + TILE_SIZE, ((i-i%longueur_)/longueur_ + 1)*TILE_SIZE);
		tempo.texCoords = textuPos2;
		tempoArray.append(tempo);

		tempo.position = sf::Vector2f((i%longueur_+1)*TILE_SIZE + TILE_SIZE, ((i-i%longueur_)/longueur_ + 1)*TILE_SIZE + TILE_SIZE);
		tempo.texCoords = textuPos3;
		tempoArray.append(tempo);

		tempo.position = sf::Vector2f((i%longueur_+1)*TILE_SIZE, ((i-i%longueur_)/longueur_ + 1)*TILE_SIZE + TILE_SIZE);
		tempo.texCoords = textuPos4;
		tempoArray.append(tempo);

		if(fin) break;
	}

	fin = false;
	for(int i(initialPos+1); i <= maxX; ++i){

		if(data_[i] == 1){
			//data_[i] = 0;
			//blocks_[i%longueur_].erase((i-i%longueur_)/longueur_);
			fin = true;
		}
		else if(data_[i] == 2 || data_[i] == -3){
			break;
		}
		else if(data_[i] == 3){
			deleteBomb(i%longueur_, (i-i%longueur_)/longueur_);
			break;
		}

		tempo.position = sf::Vector2f((i%longueur_+1)*TILE_SIZE, ((i-i%longueur_)/longueur_ + 1)*TILE_SIZE);
		tempo.texCoords = textuPos1;
		tempoArray.append(tempo);

		tempo.position = sf::Vector2f((i%longueur_+1)*TILE_SIZE + TILE_SIZE, ((i-i%longueur_)/longueur_ + 1)*TILE_SIZE);
		tempo.texCoords = textuPos2;
		tempoArray.append(tempo);

		tempo.position = sf::Vector2f((i%longueur_+1)*TILE_SIZE + TILE_SIZE, ((i-i%longueur_)/longueur_ + 1)*TILE_SIZE + TILE_SIZE);
		tempo.texCoords = textuPos3;
		tempoArray.append(tempo);

		tempo.position = sf::Vector2f((i%longueur_+1)*TILE_SIZE, ((i-i%longueur_)/longueur_ + 1)*TILE_SIZE + TILE_SIZE);
		tempo.texCoords = textuPos4;
		tempoArray.append(tempo);

		if(fin) break;
	}

	fin = false;
	for(int i(initialPos-longueur_); i >= minY; i -= longueur_){

		if(data_[i] == 1){
			//data_[i] = 0;
			//blocks_[i%longueur_].erase((i-i%longueur_)/longueur_);
			fin = true;
		}
		else if(data_[i] == 2 || data_[i] == -3){
			break;
		}
		else if(data_[i] == 3){
			deleteBomb(i%longueur_, (i-i%longueur_)/longueur_);
			break;
		}

		tempo.position = sf::Vector2f((i%longueur_+1)*TILE_SIZE, ((i-i%longueur_)/longueur_ + 1)*TILE_SIZE);
		tempo.texCoords = textuPos1;
		tempoArray.append(tempo);

		tempo.position = sf::Vector2f((i%longueur_+1)*TILE_SIZE + TILE_SIZE, ((i-i%longueur_)/longueur_ + 1)*TILE_SIZE);
		tempo.texCoords = textuPos2;
		tempoArray.append(tempo);

		tempo.position = sf::Vector2f((i%longueur_+1)*TILE_SIZE + TILE_SIZE, ((i-i%longueur_)/longueur_ + 1)*TILE_SIZE + TILE_SIZE);
		tempo.texCoords = textuPos3;
		tempoArray.append(tempo);

		tempo.position = sf::Vector2f((i%longueur_+1)*TILE_SIZE, ((i-i%longueur_)/longueur_ + 1)*TILE_SIZE + TILE_SIZE);
		tempo.texCoords = textuPos4;
		tempoArray.append(tempo);

		if(fin) break;
	}

	fin = false;
	for(int i(initialPos+longueur_); i <= maxY; i += longueur_){

		if(data_[i] == 1){
			//data_[i] = 0;
			//blocks_[i%longueur_].erase((i-i%longueur_)/longueur_);
			fin = true;
		}
		else if(data_[i] == 2 || data_[i] == -3){
			break;
		}
		else if(data_[i] == 3){
			deleteBomb(i%longueur_, (i-i%longueur_)/longueur_);
			break;
		}

		tempo.position = sf::Vector2f((i%longueur_+1)*TILE_SIZE, ((i-i%longueur_)/longueur_ + 1)*TILE_SIZE);
		tempo.texCoords = textuPos1;
		tempoArray.append(tempo);

		tempo.position = sf::Vector2f((i%longueur_+1)*TILE_SIZE + TILE_SIZE, ((i-i%longueur_)/longueur_ + 1)*TILE_SIZE);
		tempo.texCoords = textuPos2;
		tempoArray.append(tempo);

		tempo.position = sf::Vector2f((i%longueur_+1)*TILE_SIZE + TILE_SIZE, ((i-i%longueur_)/longueur_ + 1)*TILE_SIZE + TILE_SIZE);
		tempo.texCoords = textuPos3;
		tempoArray.append(tempo);

		tempo.position = sf::Vector2f((i%longueur_+1)*TILE_SIZE, ((i-i%longueur_)/longueur_ + 1)*TILE_SIZE + TILE_SIZE);
		tempo.texCoords = textuPos4;
		tempoArray.append(tempo);

		if(fin) break;
	}

	data_[initialPos] = 0;

	std::thread cleaner(&Carte::cleanSmoke, this, tempoDeb);
	cleaner.detach();
}

void Carte::addBonus(int const x, int const y, int const type){
	bonus_[x].emplace(std::make_pair(y, Bonus(&textureBonus_, x, y, type)));
	//data_[x + y * longueur_] = 3;
}

void Carte::deleteBonus(int const x, int const y){
	bonus_[x].erase(y);
}

void Carte::clear(){
	data_.clear();
	longueur_ = 0;
	hauteur_ = 0;
	mur_.clear();
	sol_.clear();

	blocks_.clear();
	bombs_.clear();
	bonus_.clear();
	
	//On ne clear pas la smokes pour eviter tout conflit avec un thread de clear de fumé, qui va s'occuper du clear de la fumer de toute manière.
	//smokes_.clear();
}

void Carte::draw(sf::RenderWindow& app){
	app.draw(sol_, &texture_);
	app.draw(mur_, &texture_);

	for(auto d = blocks_.begin(), f = blocks_.end(); d != f; ++d){
		for(auto d2(d->second.begin()), f2(d->second.end()); d2 != f2; ++d2){
			app.draw(d2->second.getSprite());
		}
	}

	for(auto d = bombs_.begin(), f = bombs_.end(); d != f; ++d){
		for(auto d2(d->second.begin()), f2(d->second.end()); d2 != f2; ++d2){
			app.draw(d2->second.getSprite());
		}
	}

	for(auto d = bonus_.begin(), f = bonus_.end(); d != f; ++d){
		for(auto d2(d->second.begin()), f2(d->second.end()); d2 != f2; ++d2){
			app.draw(d2->second.getSprite());
		}
	}

	smokeDraw.lock();
	for(auto d = smokes_.begin(), f = smokes_.end(); d != f; ++d){
		app.draw(*d, &textureSmoke_);
	}
	smokeDraw.unlock();
}