#include "head.h"


using namespace sf;

int menu(RenderWindow& app){
	size_t indexChoix(0);
	size_t i, taille, tailleInfo;
	bool modeChoix(true);
	Text* choix;
	Clock temps;

	Texture fondecran_;
	Sprite fondecran;
	Sprite fondecran2;

	if(!fondecran_.loadFromFile("ressources/sprites/menupaper.png")){

	}
	else{
		fondecran.setTexture(fondecran_);
		fondecran.setPosition(0, -400);

		fondecran2.setTexture(fondecran_);
		fondecran2.setPosition(1920, -400);
	}

	Font font;
	std::vector<std::unique_ptr<Text>> texts;
	std::vector<std::unique_ptr<Text>> infos;

	if(!font.loadFromFile("ressources/fonts/Avara.ttf")){
		return -1;
	}
	else{
		int tempoX(app.getSize().x/2), tempoY(app.getSize().y);

		//Pour les choix
		texts.emplace_back(std::make_unique<Text>(L"Rejoindre une partie", font));
		texts.back()->setOrigin({ texts.back()->getLocalBounds().width/2, texts.back()->getLocalBounds().height/2 });
		texts.back()->setPosition(tempoX, tempoY*5/10 - tempoY/20);
		texts.back()->setFillColor(Color::Black);

		choix = texts.back().get();
		choix->setFillColor(Color::White);

		texts.emplace_back(std::make_unique<Text>(L"Héberger", font));
		texts.back()->setOrigin({ texts.back()->getLocalBounds().width/2, texts.back()->getLocalBounds().height/2 });
		texts.back()->setPosition(tempoX, tempoY*7/10 - tempoY/20);
		texts.back()->setFillColor(Color::Black);

		//Info sur les ip
		auto infoIp(findMyIp());

		infos.emplace_back(std::make_unique<Text>(L"Bomberman", font, 50));
		infos.back()->setOrigin({ infos.back()->getLocalBounds().width/2, infos.back()->getLocalBounds().height/2 });
		infos.back()->setPosition(tempoX, tempoY*2/10 - tempoY/20);
		infos.back()->setFillColor(Color(200, 66, 244));
		infos.back()->setOutlineThickness(3);
		infos.back()->setOutlineColor(sf::Color(3627355));

		infos.emplace_back(std::make_unique<Text>(L"Par Guillaume\nMagniadas", font, 20));
		infos.back()->setOrigin({ infos.back()->getLocalBounds().width/2, infos.back()->getLocalBounds().height/2 });
		infos.back()->setPosition(tempoX + tempoX/1.5, tempoY*8/10 - tempoY/20);
		infos.back()->setFillColor(Color::Black);
		infos.back()->setOutlineThickness(3);
		infos.back()->setOutlineColor(sf::Color(3627355));

		infos.emplace_back(std::make_unique<Text>("Ip Local : " + infoIp[0], font));
		//infos.back()->setOrigin({ infos.back()->getLocalBounds().width/2, infos.back()->getLocalBounds().height/2 });
		infos.back()->setPosition(5, tempoY - 35);
		infos.back()->setFillColor(Color::White);

		infos.back()->setOutlineThickness(3);
		infos.back()->setOutlineColor(sf::Color(3627355));

		infos.back()->setOutlineThickness(3);
		infos.back()->setOutlineColor(sf::Color(3627355));

	}
	taille = texts.size();
	tailleInfo = infos.size();



	Event event;

	temps.restart();
	while(app.isOpen()){

		sf::Vector2f tempo;

		while(app.pollEvent(event)){
			if(event.type == Event::Closed){
				app.close();
			}
			if(event.type == Event::KeyPressed){
				switch(event.key.code){

					case Keyboard::Up :
						if(indexChoix == 0) indexChoix=taille-1;
						else --indexChoix;
						break;

					case Keyboard::Z :
						if(indexChoix == 0) indexChoix=taille-1;
						else --indexChoix;
						break;

					case Keyboard::Down :
						if(indexChoix == taille-1) indexChoix = 0;
						else ++indexChoix;
						break;

					case Keyboard::S :
						if(indexChoix == taille-1) indexChoix = 0;
						else ++indexChoix;
						break;

					case Keyboard::Enter :
						return indexChoix+1;
						break;

					case Keyboard::Escape :
						app.close();
						break;

					default:
						break;
				}
			}
		}
		
		if(texts[indexChoix].get() != choix){
			temps.restart();
			modeChoix=true;
			choix->setScale(1, 1);
			choix->setFillColor(Color::Black);
			choix = texts[indexChoix].get();
			choix->setFillColor(Color::White);

		}

		if(temps.getElapsedTime().asSeconds()>0.01){
			temps.restart();
			tempo = choix->getScale();
			if(modeChoix){
				if(tempo.x >= 1.2){
					modeChoix=false;
					choix->setScale(tempo.x-0.01, tempo.y-0.01);
				}
				else{
					choix->setScale(tempo.x+0.01, tempo.y+0.01);
				}
			}
			else{
				if(tempo.x <= 0.7){
					modeChoix=true;
					choix->setScale(tempo.x+0.01, tempo.y+0.01);
				}
				else{
					choix->setScale(tempo.x-0.01, tempo.y-0.01);
				}
			}
		}


		if(fondecran.getPosition().x <= -1920){
			fondecran.setPosition(fondecran2.getPosition().x+1920, -400);
		}
		if(fondecran2.getPosition().x == -1920){
			fondecran2.setPosition(fondecran.getPosition().x+1920, -400);
		}
		fondecran.move(-2, 0);
		fondecran2.move(-2, 0);

		app.draw(fondecran);
		app.draw(fondecran2);

		for(i=0; i<taille; ++i){
			app.draw(*texts[i]);
		}

		for(i=0; i<tailleInfo; ++i){
			app.draw(*infos[i]);
		}

		app.display();
	}
	return 0;
}


//boite a question
int menu_ask(sf::RenderWindow& app, sf::String asking_text, std::string& tempo){

	//sf::RenderWindow ask;

	sf::String playerInput;
	sf::Font font;

	if(!font.loadFromFile("ressources/fonts/Avara.ttf")){
		return -1;
	}

	//Toute la partie sur le texte
	sf::Text playerText("", font);
	playerText.setFillColor(sf::Color::Black);
	playerText.setOutlineThickness(2);
	playerText.setOutlineColor(sf::Color(3627355));
	playerText.setPosition(0, 100);

	sf::Text asking(asking_text, font);
	asking.setFillColor(sf::Color::Black);
	asking.setOutlineThickness(2);
	asking.setOutlineColor(sf::Color(3627355));

	asking.setOrigin(sf::Vector2f(static_cast<int>(asking.getLocalBounds().width/2), static_cast<int>(asking.getLocalBounds().height/2)));
	asking.setPosition(app.getView().getCenter().x, 50);

	sf::Event event;
	while(app.isOpen()){
		while(app.pollEvent(event)){
			if(event.type == sf::Event::Closed){
				app.close();
				return 2;
			}

			if (event.type == sf::Event::TextEntered){
				if (event.text.unicode == '\b'){
					if(playerInput.getSize() != 0) playerInput.erase(playerInput.getSize() - 1, 1);
				}
				else if(event.text.unicode == ' ' || event.text.unicode == '\r' || event.text.unicode == '\n'){
					break;
				}
				else if(event.text.unicode < 128)
					playerInput += event.text.unicode;
				playerText.setString(playerInput);
				playerText.setOrigin(sf::Vector2f(static_cast<int>(playerText.getLocalBounds().width/2), static_cast<int>(playerText.getLocalBounds().height/2)));
				playerText.setPosition(app.getView().getCenter());
			}
			if(event.type == sf::Event::KeyPressed){
				switch(event.key.code){
					case sf::Keyboard::Escape :
						return 2;

					case sf::Keyboard::Enter :
						tempo = playerInput.toAnsiString();
						return 1;

					default:
						break;
				}
			}
		}

		app.clear(sf::Color(10732742));

		app.draw(asking);
		app.draw(playerText);

		app.display();

	}

	return 0;
}
