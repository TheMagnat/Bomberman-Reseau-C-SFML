#include "Case.hpp"
#include <iostream>
Case::Case(sf::Vector2f const& pos, sf::IntRect const& textureRect, int type)
		: type_(type), pos_(pos), hitBox_(pos.x, pos.y, TILE_SIZE, TILE_SIZE), textureRect_(textureRect) {}


Block::Block(sf::Texture* const& texture, sf::Vector2f const& pos, sf::IntRect const& textureRect, int const& type)
		: Case(pos, textureRect, 2), sprite_(*texture, textureRect_) {
	sprite_.setPosition(pos_);
}

sf::Sprite& Block::getSprite(){
	return sprite_;
}

