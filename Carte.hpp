#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <list>
#include <unordered_map>
#include <fstream>
#include <iostream>

#include <thread>
#include <mutex>

//Chrono null
#include <unistd.h>

#include "SoundHandler.hpp"

#include "macro.h"
#include "Case.hpp"
#include "StringToVec.hpp"
#include "Bomb.hpp"
#include "Bonus.hpp"

class Carte {
	public:

		Carte(std::string const& fichierTexture, std::string const& fichierTextureBomb);
		Carte(std::string const& nomFichier, std::string const& fichierTexture, std::string const& fichierTextureBomb);

		void load(const char * data);
		void breakWall(const char * data);

		void addaptMap(sf::RenderWindow& app);

		void cleanSmoke(std::list<sf::VertexArray>::iterator it);
		std::list<sf::VertexArray>::iterator addSmoke(int x, int minX, int maxX, int y, int minY, int maxY);

		void addBomb(int x, int y, int pow);
		void deleteBomb(int x, int y);

		void addBonus(int const x, int const y, int const type);
		void deleteBonus(int const x, int const y);

		void clear();

		void draw(sf::RenderWindow& app);

	private:
		sf::Texture texture_;
		sf::Texture textureBomb_;
		sf::Texture textureSmoke_;
		sf::Texture textureBonus_;

		std::vector<int> data_;
		size_t longueur_;
		size_t hauteur_;
		sf::VertexArray mur_;
		sf::VertexArray sol_;

		std::unordered_map<int, std::unordered_map<int, Block>> blocks_;
		std::unordered_map<int, std::unordered_map<int, Bomb>> bombs_;
		std::unordered_map<int, std::unordered_map<int, Bonus>> bonus_;
		std::list<sf::VertexArray> smokes_;

		//Mutex pour empecher de supprimmer la smoke quand un draw de cette dernière est en cours.
		std::mutex smokeDraw;
};