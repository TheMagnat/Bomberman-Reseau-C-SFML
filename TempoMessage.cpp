#include "TempoMessage.hpp"
#include <iostream>

TempoMessage::TempoMessage() : actif_(false), full_(false), transparence_(0) {

	if(!font_.loadFromFile("ressources/fonts/Avara.ttf")){
		std::cerr << "Impossible de charger 'ressources/fonts/Avara.ttf'" << std::endl;
	}

	message_.setFont(font_);
	message_.setOutlineThickness(1);

}

void TempoMessage::setNewMessage(std::wstring const& message, sf::Vector2f const& position, float timeAliveMilli){
	actif_ = true;
	full_ = true;

	message_.setString(message);
	message_.setOrigin({ message_.getLocalBounds().width/2, message_.getLocalBounds().height/2 });
	message_.setPosition(position);

	aliveTimerMilli_ = timeAliveMilli;
	aliveClock_.restart();

	transparence_ = 255;
}

void TempoMessage::draw(sf::RenderWindow& app){

	if(actif_){
		if(!full_){
			transparence_ -= 5;
			if(transparence_ <= 0) actif_ = false;
		}
		else{
			if(aliveTimerMilli_ < aliveClock_.getElapsedTime().asMilliseconds()){
				full_ = false;
				transparence_ -= 5;
			}
		}
	}

	message_.setFillColor(sf::Color(0, 0, 0, transparence_));
	message_.setOutlineColor(sf::Color(255, 255, 255, transparence_));

	app.draw(message_);
}