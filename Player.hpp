#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "macro.h"

#include <string>

class Player {

	public:
		explicit Player(sf::Texture* texture, std::string const& pseudo, bool our = false);
		explicit Player(sf::Texture* texture, int x, int y, std::string const& pseudo, bool our = false);

		std::string const& getName() const;

		std::wstring getWName() const;

		void setPosition(int x, int y);

		void killed();

		void putAlive();

		void draw(sf::RenderWindow& app);

	private:
		bool alive_;

		std::string pseudo_;

		sf::Texture* texture_;
		sf::Sprite sprite_;

		bool our_;
};