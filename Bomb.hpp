#pragma once

#include <SFML/Graphics.hpp>
#include "macro.h"

class Bomb {
public:

	Bomb(sf::Texture* texture, int xx, int yy, int powerr);

	int getPow();
	sf::Sprite& getSprite();


private:
	int x_;
	int y_;
	int power_;

	sf::Sprite sprite_;
};