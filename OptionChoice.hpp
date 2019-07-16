#pragma once

#include <SFML/Graphics.hpp>
#include <array>

#include "macro.h"

class OptionChoice {
public:

	OptionChoice();

	void setAll(std::array<int, 9> const& allOption);

	bool checkClick(sf::Vector2f const& sourieClick);

	void draw(sf::RenderWindow& app);

	std::string toSend_;

private:

	void fillSend(size_t numOption, bool increase);

	sf::Texture clicksTexture_;
	sf::Font font_;

	std::array<int, 9> options_;
	std::array<int, 9> optionsStep_;
	std::array<sf::Text, 9> optionsInfo_;
	std::array<sf::Text, 9> optionsText_;
	std::array<sf::Sprite, 18> optionsClick_;
	


};