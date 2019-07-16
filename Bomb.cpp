#include "Bomb.hpp"

Bomb::Bomb(sf::Texture* texture, int xx, int yy, int powerr) : x_(xx), y_(yy), power_(powerr) {
	sprite_.setTexture(*texture);
	sprite_.setPosition((x_+1)*TILE_SIZE, (y_+1)*TILE_SIZE);
}


int Bomb::getPow(){
	return power_;
}

sf::Sprite& Bomb::getSprite(){
	return sprite_;
}