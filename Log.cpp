#include "Log.hpp"

#include <iostream>

Log::Log(int size) : size_(size) {
	if(!font_.loadFromFile("ressources/fonts/Avara.ttf")){
		std::cerr << "Impossible de charger ressources/fonts/Avara.ttf\n";
	}
}


void Log::addMessage(std::wstring const& newMessage, sf::Color const& newCol){

	for(size_t i(0), taille(allMessages.size()); i < taille; ++i){
		allMessages[i].move(0, size_+5);
	}

	allMessages.emplace_back(newMessage, font_, size_);
	allMessages.back().setFillColor(newCol);
	allMessages.back().setOutlineColor(sf::Color::Black);
	allMessages.back().setOutlineThickness(1);
}

void Log::clear(){
	allMessages.clear();
}

void Log::draw(sf::RenderWindow& app){

	for(size_t i(0), taille(allMessages.size()); i < taille; ++i){

		app.draw(allMessages[i]);

	}

}