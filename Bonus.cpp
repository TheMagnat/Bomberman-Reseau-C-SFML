#include "Bonus.hpp"

Bonus::Bonus(sf::Texture* texture, int xx, int yy, int type) : x_(xx), y_(yy), type_(type), sprite_(*texture) {

	if(type == 0){
		sprite_.setTextureRect(sf::IntRect(0, 0, 32, 32));
	}
	else if(type == 1){
		sprite_.setTextureRect(sf::IntRect(32, 0, 32, 32));
	}
	else{
		sprite_.setTextureRect(sf::IntRect(64, 0, 32, 32));
	}

	sprite_.setPosition((x_+1)*TILE_SIZE, (y_+1)*TILE_SIZE);
}

sf::Sprite& Bonus::getSprite(){
	return sprite_;
}