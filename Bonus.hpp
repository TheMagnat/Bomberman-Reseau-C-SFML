#pragma once

#include <SFML/Graphics.hpp>
#include "macro.h"

class Bonus {
public:

	Bonus(sf::Texture* texture, int xx, int yy, int type);

	sf::Sprite& getSprite();

private:
	int x_;
	int y_;
	int type_;

	sf::Sprite sprite_;
	
};