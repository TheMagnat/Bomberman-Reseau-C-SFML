#include "Player.hpp"

Player::Player(sf::Texture* texture, std::string const& pseudo, bool our) : alive_(true), pseudo_(pseudo), texture_(texture), our_(our){
	sprite_.setTexture(*texture_);
	if(our){
		sprite_.setTextureRect(sf::IntRect(320, 0, 32, 32));
	}
	else{
		sprite_.setTextureRect(sf::IntRect(32, 0, 32, 32));
	}
}

Player::Player(sf::Texture* texture, int x, int y, std::string const& pseudo, bool our) : pseudo_(pseudo), texture_(texture), our_(our){
	sprite_.setTexture(*texture_);
	sprite_.setTextureRect(sf::IntRect(32, 0, 32, 32));
	setPosition(x, y);
}
//joueur.png

std::string const& Player::getName() const{
	return pseudo_;
}

std::wstring Player::getWName() const{
    std::wstring retString(pseudo_.length(), L' ');
    
    std::copy(pseudo_.begin(), pseudo_.end(), retString.begin());
	return retString;
}

void Player::setPosition(int x, int y){
	sf::Vector2f oldPos(sprite_.getPosition());
	int oldX(oldPos.x/TILE_SIZE - 1), oldY(oldPos.y/TILE_SIZE - 1);
	int textuX;
	if(our_){
		textuX = 320;
	}
	else{
		textuX = 32;
	}
	if(x > oldX){
		sprite_.setTextureRect(sf::IntRect(textuX, 64, 32, 32));
	}
	else if(x < oldX){
		sprite_.setTextureRect(sf::IntRect(textuX, 32, 32, 32));
	}
	else if(y > oldY){
		sprite_.setTextureRect(sf::IntRect(textuX, 0, 32, 32));
	}
	else if(y < oldY){
		sprite_.setTextureRect(sf::IntRect(textuX, 96, 32, 32));
	}
	sprite_.setPosition((x+1)*TILE_SIZE, (y+1)*TILE_SIZE);
}

void Player::killed(){
	alive_ = false;
}

void Player::putAlive(){
	alive_ = true;
}

void Player::draw(sf::RenderWindow& app){
	if(alive_) app.draw(sprite_);
}