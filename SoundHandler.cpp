#include "SoundHandler.hpp"

std::vector<sf::SoundBuffer> SoundHandler::sons_;
std::vector<sf::Sound> SoundHandler::lecteur_;

void SoundHandler::initialiserSons(std::vector<std::string> fichiers){
	size_t i, taille;
	taille = fichiers.size();
	for(i=0; i<taille; ++i){
		sons_.emplace_back();
		if(!sons_.back().loadFromFile(fichiers[i])){
			std::cerr << "Impossible de charger : " << fichiers[i] << std::endl;
		}
	}

	lecteur_.emplace_back();
}

void SoundHandler::findLecteur(int indexAjouer){
	bool save(false);

	for(size_t i(0), taille(lecteur_.size()); i<taille; ++i){
		if(lecteur_[i].getStatus() == sf::SoundSource::Status::Stopped){
			lecteur_[i].setBuffer(sons_[indexAjouer]);
			lecteur_[i].play();
			save = true;
			break;
		}
	}

	if(!save){
		lecteur_.emplace_back();
		lecteur_.back().setBuffer(sons_[indexAjouer]);
		lecteur_.back().play();
	}

}