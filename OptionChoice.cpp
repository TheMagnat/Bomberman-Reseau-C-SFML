#include "OptionChoice.hpp"
#include <iostream>

OptionChoice::OptionChoice() {

	if(!clicksTexture_.loadFromFile("ressources/sprites/clicks.png")){
		std::cerr << "ressources/sprites/clicks.png\n";
	}

	if(!font_.loadFromFile("ressources/fonts/Avara.ttf")){
		std::cerr << "Impossible de charger ressources/fonts/Avara.ttf\n";
	}

	sf::IntRect plus(0, 0, 16, 16);
	sf::IntRect moins(16, 0, 16, 16);

	for(size_t i(0); i < 9; ++i){
		options_[i] = 1;

		optionsInfo_[i].setFont(font_);
		optionsInfo_[i].setCharacterSize(10);
		optionsInfo_[i].setString("1");
		optionsInfo_[i].setOutlineColor(sf::Color::Black);
		optionsInfo_[i].setOutlineThickness(1.f);

		optionsText_[i].setFont(font_);
		optionsText_[i].setString("1");
		optionsText_[i].setOutlineColor(sf::Color::Black);
		optionsText_[i].setOutlineThickness(1.f);
		optionsText_[i].setOrigin({ optionsText_[i].getLocalBounds().width/2, 0 });
		optionsText_[i].setPosition(30 + i*64, WINDOW_HEIGHT - 93);

		optionsClick_[i*2].setTexture(clicksTexture_);
		optionsClick_[i*2].setTextureRect(plus);
		optionsClick_[i*2].setOrigin({ optionsClick_[i*2].getLocalBounds().width/2, 0 });
		optionsClick_[i*2].setPosition(30 + i*64, WINDOW_HEIGHT - 106);

		optionsClick_[i*2+1].setTexture(clicksTexture_);
		optionsClick_[i*2+1].setTextureRect(moins);
		optionsClick_[i*2+1].setOrigin({ optionsClick_[i*2+1].getLocalBounds().width/2, 0 });
		optionsClick_[i*2+1].setPosition(30 + i*64, WINDOW_HEIGHT - 60);
	}

	//Nom des options
	optionsInfo_[0].setString("Bombe");
	optionsInfo_[1].setString("Explo");
	optionsInfo_[2].setString("Speed");
	optionsInfo_[3].setString("Vit Expl");
	optionsInfo_[4].setString("Vie");
	optionsInfo_[5].setString("Bns Chance");
	optionsInfo_[6].setString("Coef Bmb");
	optionsInfo_[7].setString("Coef Expl");
	optionsInfo_[8].setString("Coef Speed");

	for(size_t i(0); i < 9; ++i){
		optionsInfo_[i].setOrigin({ optionsInfo_[i].getLocalBounds().width/2, 0 });
		optionsInfo_[i].setPosition(30 + i*64, WINDOW_HEIGHT - 136);
	}

	//Les steps des options
	optionsStep_[0] = 1;
	optionsStep_[1] = 1;
	optionsStep_[2] = 1;
	optionsStep_[3] = 100;
	optionsStep_[4] = 1;
	optionsStep_[5] = 5;
	optionsStep_[6] = 1;
	optionsStep_[7] = 1;
	optionsStep_[8] = 1;

}

void OptionChoice::setAll(std::array<int, 9> const& allOption){

	for(size_t i(0); i < 9; ++i){

		optionsText_[i].setString(std::to_string(allOption[i]));
		optionsText_[i].setOrigin({ optionsText_[i].getLocalBounds().width/2, 0 });
		optionsText_[i].setPosition(30 + i*64, WINDOW_HEIGHT - 93);

		options_[i] = allOption[i];

	}


}

void OptionChoice::fillSend(size_t numOption, bool increase){

	int mult(1);

	if(!increase){
		mult = -1;
	}

	if(numOption < 5){
		toSend_ = "SET";

		for(size_t i(0); i < 5; ++i){

			toSend_.push_back(' ');

			if(i == numOption){
				toSend_ += std::to_string(options_[i] + optionsStep_[i]*mult);
			}
			else{
				toSend_ += std::to_string(options_[i]);
			}

		}

		toSend_.push_back('\n');
	}
	else{
		toSend_ = "CHA";

		for(size_t i(5); i < 9; ++i){
			
			toSend_.push_back(' ');

			if(i == numOption){
				toSend_ += std::to_string(options_[i] + optionsStep_[i]*mult);
			}
			else{
				toSend_ += std::to_string(options_[i]);
			}

		}

		toSend_.push_back('\n');

	}


}

bool OptionChoice::checkClick(sf::Vector2f const& sourieClick){

	for(size_t i(0); i < 9; ++i){

		if(optionsClick_[i*2].getGlobalBounds().contains(sourieClick)){
			fillSend(i, true);
			return true;
		}
		else if(optionsClick_[i*2+1].getGlobalBounds().contains(sourieClick)){
			fillSend(i, false);
			return true;
		}

	}

	return false;
}


void OptionChoice::draw(sf::RenderWindow& app){

	for(size_t i(0); i < 9; ++i){
		app.draw(optionsInfo_[i]);
		app.draw(optionsText_[i]);
		app.draw(optionsClick_[i*2]);
		app.draw(optionsClick_[i*2 + 1]);
	}

}