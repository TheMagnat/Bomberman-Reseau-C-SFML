#include "head.h"
#include "multiplayer.h"
#include "Player.hpp"
#include "TempoMessage.hpp"
#include <unordered_map>


#include <thread>
#include <experimental/filesystem>


int partie(sf::RenderWindow& app, Carte& map, std::unordered_map<int, Player>& joueurs, Protocole& myProto){

	int save;
	bool musicIsOpen(false), restart(false);


	//Les messages
	TempoMessage screenMessage;


	//Musique

	std::vector<std::string> allMusic;

    std::experimental::filesystem::directory_iterator myIte("ressources/music/game");
    for(auto it = std::experimental::filesystem::begin(myIte), itEnd = std::experimental::filesystem::end(myIte); it != itEnd; ++it)
         allMusic.push_back(it->path());

    //Choix random de la musique
	sf::Music music;

	if(!allMusic.empty()){
		std::string selectedPath(allMusic[rand()%allMusic.size()]); 
		if(!music.openFromFile(selectedPath))
		    std::cerr << selectedPath << std::endl;
		else{
			music.setLoop(true);
			musicIsOpen = true;
		}
	}

	sf::Clock timer_;
	sf::Clock timer2_;

	//On lance la musique au début de la partie
	if(musicIsOpen) music.play();
	sf::Event event;

	map.addaptMap(app);

	screenMessage.setNewMessage(L"Vous êtes le personnage bleue !", app.getView().getCenter(), 1000);

	while(app.isOpen()){
		while(app.pollEvent(event)){
			if(event.type == sf::Event::Closed){
				app.close();
				return 1;
			}
			if(event.type == sf::Event::KeyPressed){
				switch(event.key.code){

					case sf::Keyboard::Escape :
						app.setView(app.getDefaultView());

						//Solution du pauvre pour s'assurer que tout les thread temporaire ai fini
						std::this_thread::sleep_for(std::chrono::milliseconds(1100));
						return 0;

					case sf::Keyboard::Return :
						if(restart) myProto.sendRST(0);
						break;

					default:
						break;
				}
			}
			if (event.type == sf::Event::Resized){
			   	map.addaptMap(app);
			}

		}

		while((save = myProto.duringGame()) > 0){
			if(save == 1){
				joueurs.at(myProto.tempoNum).setPosition(myProto.tempoX, myProto.tempoY);
			}
			else if(save == 2){
				map.addBomb(myProto.tempoX, myProto.tempoY, myProto.tempoNum);
			}
			else if(save == 3){
				map.deleteBomb(myProto.tempoX, myProto.tempoY);
			}
			else if(save == 4){
				map.breakWall(myProto.buff_ + 4);
			}
			else if(save == 5){
				map.addBonus(myProto.tempoX, myProto.tempoY, myProto.tempoNum);
			}
			else if(save == 6){
				map.deleteBonus(myProto.tempoX, myProto.tempoY);
			}
			else if(save == 7){
				joueurs.at(myProto.tempoX).killed();
				if(myProto.tempoX == myProto.tempoY){
					screenMessage.setNewMessage(joueurs.at(myProto.tempoY).getWName() + L" en avait marre de la vie :( ", app.getView().getCenter(), 1000);
				}
				else{
					screenMessage.setNewMessage(joueurs.at(myProto.tempoY).getWName() + L" à fait exploser " + joueurs.at(myProto.tempoX).getWName() + L" !", app.getView().getCenter(), 1000);
				}
			}
			else if(save == 8){
				restart = true;
				if(myProto.tempoNum == -1){
					screenMessage.setNewMessage(L"Suicide général ! Personne ne gagne.", app.getView().getCenter(), 3000);
				}
				else{
					screenMessage.setNewMessage(L"Victoire de " + joueurs.at(myProto.tempoNum).getWName() + L" !", app.getView().getCenter(), 3000);
				}
			}
			else if(save == 9){
				app.setView(app.getDefaultView());
				music.stop();
				return 2;
			}
			else if(save == 10){
				joueurs.erase(myProto.tempoNum);
			}
		}
		if(save == -1){
			app.setView(app.getDefaultView());

			//Solution du pauvre pour s'assurer que tout les thread temporaire ai fini
			std::this_thread::sleep_for(std::chrono::milliseconds(1100));
			return 0;
		}

		if(timer_.getElapsedTime().asSeconds() > 0.025){
			bool stop = false;
			uint8_t x = 0;
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)){
				x = 1;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
				if(x) stop = true;
				x = 2;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
				if(x) stop = true;
				x = 3;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
				if(x) stop = true;
				x = 4;
			}
			if(!stop){
				if(x){
					timer_.restart();
					myProto.sendMove(x);
				}
			}
		}
		if(timer2_.getElapsedTime().asSeconds() > 0.025){
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
				timer2_.restart();
				myProto.sendBomb();
			}
		}


		app.clear(sf::Color(21, 108, 153));

		map.draw(app);

		for(auto it(joueurs.begin()), itEnd(joueurs.end()); it != itEnd; ++it){
			it->second.draw(app);
		}

		screenMessage.draw(app);

		app.display();
	}

	music.stop();
	return 1;
}