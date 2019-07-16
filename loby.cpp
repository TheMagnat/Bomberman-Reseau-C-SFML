#include "head.h"
#include "multiplayer.h"
#include "Player.hpp"
#include "Log.hpp"
#include "SoundHandler.hpp"
#include "OptionChoice.hpp"
#include <unordered_map>
#include <cmath>
#include <thread>
#include <algorithm>
#include <codecvt>


int beforeGoingToLoby(sf::RenderWindow& app, int sock){
	std::unordered_map<int, Player> joueurs;
	return loby(app, sock, joueurs);
}

int loby(sf::RenderWindow& app, int sock, std::unordered_map<int, Player>& joueurs){

	int ourNum(0), save;
	uint8_t phase(1);
	bool start(false);

	Log logs(30);

	Carte map("ressources/sprites/forest_tiles.png", "ressources/sprites/bomb.png");
	Protocole myProto(sock);

	sf::Texture textureJoueur;
	textureJoueur.loadFromFile("ressources/sprites/perso.png");

	sf::Music music;
	if(!music.openFromFile("ressources/music/loby.ogg"))
	    std::cerr << "ressources/music/loby.ogg" << std::endl;
	else{
		music.setLoop(true);
	}

	//La partie pour le fond d'écran
	sf::Texture fondecran_;
	sf::Sprite fondecran;
	sf::Sprite fondecran2;
	sf::Sprite fondecran3;
	sf::Sprite fondecran4;

	std::vector<sf::Sprite> fondecrans;

	if(!fondecran_.loadFromFile("ressources/sprites/lobypaper.png")){

	}
	else{

		for(size_t i(0); i < 4; ++i){
			fondecrans.emplace_back(fondecran_);
			fondecrans.back().scale(2, 2);
		}

		fondecrans[0].setPosition(0, 0);

		fondecrans[1].setPosition(1051*2, 0);

		fondecrans[2].setPosition(0, 1051*2);

		fondecrans[3].setPosition(1051*2, 1051*2);
	}

	//Le chat

	sf::Font font;

	if(!font.loadFromFile("ressources/fonts/Avara.ttf")){
		std::cerr << "Impossible de charger ressources/fonts/Avara.ttf\n";
	}

	sf::RectangleShape chatWindow(sf::Vector2f(WINDOW_WIDTH, 40));
	chatWindow.setPosition(0, WINDOW_HEIGHT - 40);
	chatWindow.setFillColor(sf::Color(0, 0, 0));

	std::string tempoString;
	sf::String playerInput;

	sf::Text chatText("", font);
	chatText.setFillColor(sf::Color::White);
	chatText.setPosition(0, WINDOW_HEIGHT - 35);


	//Bouton start
	sf::CircleShape startButton(50);
	startButton.setPosition(WINDOW_WIDTH - 110, 10);
	startButton.setFillColor(sf::Color::Green);
	startButton.setOutlineColor(sf::Color::Black);
	startButton.setOutlineThickness(10);

	sf::Text startText("Start", font);
	startText.setFillColor(sf::Color::Black);
	startText.setOrigin({ startText.getLocalBounds().width/2, startText.getLocalBounds().height/2 });
	startText.setPosition(WINDOW_WIDTH - 110 + 50, 10 + 40);

	//Bouton Clear
	sf::CircleShape clearButton(50);
	clearButton.setPosition(WINDOW_WIDTH - 110, 100);
	clearButton.setFillColor(sf::Color::Blue);
	clearButton.setOutlineColor(sf::Color::Black);
	clearButton.setOutlineThickness(10);

	sf::Text clearText("Clear", font);
	clearText.setFillColor(sf::Color::Black);
	clearText.setOrigin({ clearText.getLocalBounds().width/2, clearText.getLocalBounds().height/2 });
	clearText.setPosition(WINDOW_WIDTH - 110 + 50, 100 + 40);

	//Map choisi

	sf::Text mapText(L"Map numéro : ", font);
	mapText.setFillColor(sf::Color::Black);
	mapText.setPosition(WINDOW_WIDTH - mapText.getLocalBounds().width - 100, WINDOW_HEIGHT - 70);
	mapText.setOutlineColor(sf::Color::White);
	mapText.setOutlineThickness(1);

	sf::Text mapName(L"-Basique", font);
	mapName.setFillColor(sf::Color::Black);
	mapName.setPosition(WINDOW_WIDTH - mapText.getLocalBounds().width - 100, WINDOW_HEIGHT - 105);
	mapName.setOutlineColor(sf::Color::White);
	mapName.setOutlineThickness(1);

	sf::Text mapNum(L"0", font);
	mapNum.setFillColor(sf::Color::Black);
	mapNum.setPosition(WINDOW_WIDTH - 100, WINDOW_HEIGHT - 70);
	mapNum.setOutlineColor(sf::Color::White);
	mapNum.setOutlineThickness(1);

	//Options
	OptionChoice mesOptions;

	sf::Clock test;

	//On lance la musique du loby
	music.play();
	sf::Event event;
	while(app.isOpen()){
		while(app.pollEvent(event)){
			if(event.type == sf::Event::Closed){
				app.close();
				return 1;
			}
			if(event.type == sf::Event::MouseButtonPressed){

			    if(event.mouseButton.button == sf::Mouse::Left){
			    	sf::Vector2f tempo = app.mapPixelToCoords(sf::Mouse::getPosition(app));
			        if(clearButton.getGlobalBounds().contains(tempo)){
			        	logs.clear();
			        }
			        else if(startButton.getGlobalBounds().contains(tempo)){
			        	send(sock, "RDY\n", 4, 0);
			        }
			        else{
			        	if(mesOptions.checkClick(tempo)){
			        		send(sock, mesOptions.toSend_.data(), mesOptions.toSend_.size(), 0);
			        	}
			        }
			    }

			}
			if(event.type == sf::Event::KeyPressed){
				switch(event.key.code){

					case sf::Keyboard::Escape :
						return 0;

					case sf::Keyboard::Return :

						if(playerInput.isEmpty()) break;

						if(true)
						{
							size_t isIn = playerInput.find("/set ");
							if(isIn == 0){
								playerInput.erase(0, 4);
								tempoString = "SET" + playerInput.toAnsiString() + "\n";
								send(sock, tempoString.data(), tempoString.size(), 0);
								playerInput.clear();
								chatText.setString(playerInput);
								break;
							}
						}

						{
							size_t isIn = playerInput.find("/chance ");
							if(isIn == 0){
								playerInput.erase(0, 7);
								tempoString = "CHA" + playerInput.toAnsiString() + "\n";
								send(sock, tempoString.data(), tempoString.size(), 0);
								playerInput.clear();
								chatText.setString(playerInput);
								break;
							}
						}

						tempoString = "MSG " + playerInput.toAnsiString() + "\n";
						send(sock, tempoString.data(), tempoString.size(), 0);
						playerInput.clear();
						chatText.setString(playerInput);
						break;

					case sf::Keyboard::Up :
						send(sock, "RDY\n", 4, 0);
						break;

					case sf::Keyboard::Right :
						send(sock, "NXT\n", 4, 0);
						break;

					case sf::Keyboard::Left :
						send(sock, "PRV\n", 4, 0);
						break;

					default:
						break;
				}
			}
			if (event.type == sf::Event::TextEntered){
				if (event.text.unicode == '\b'){
					if(playerInput.getSize() != 0) playerInput.erase(playerInput.getSize() - 1, 1);
				}
				else if(event.text.unicode == '\t' || event.text.unicode == '\r' || event.text.unicode == '\n') continue;
				else if(event.text.unicode < 128) playerInput += event.text.unicode;
				chatText.setString(playerInput);
			}
			if (event.type == sf::Event::Resized){

			}
		}
		if(phase == 1){
			//En attente des connections
			while((save = myProto.beforeGameRecv()) > 0){
				if(save == 0){
					continue;
				}
				else if(save == 1){
					std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> conv;
					std::wstring myConv = conv.from_bytes(myProto.tempoBuff);
					logs.addMessage(myConv + L" vient de se connecter !", sf::Color::Green);
					SoundHandler::playConnect();
					if(myProto.tempoNum == ourNum){
						joueurs.emplace(std::make_pair(myProto.tempoNum, Player(&textureJoueur, myProto.tempoBuff, true)));
					}
					else{
						joueurs.emplace(std::make_pair(myProto.tempoNum, Player(&textureJoueur, myProto.tempoBuff)));
					}
				}
				else if(save == 2){
					std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> conv;
					std::wstring myConv = conv.from_bytes(joueurs.at(myProto.tempoNum).getName());
					logs.addMessage(myConv + L" vient de se déconnecter :(", sf::Color::Red);
					SoundHandler::playDisconnect();
					joueurs.erase(myProto.tempoNum);
				}
				else if(save == 3){
					ourNum = myProto.tempoNum;
				}
				else if(save == 4){
					std::vector<std::pair<int, std::string>> tempoPlayers;

					myProto.fillLst(tempoPlayers);

					for(size_t i(0), tempoSize(tempoPlayers.size()); i < tempoSize; ++i){
						std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> conv;
						std::wstring myConv = conv.from_bytes(tempoPlayers[i].second);
						logs.addMessage(myConv + L" est présent", sf::Color::Green);
						joueurs.emplace(std::make_pair(tempoPlayers[i].first, Player(&textureJoueur, tempoPlayers[i].second)));
					}
				}
				else if(save == 5){
					std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> conv;
					std::wstring myConv = conv.from_bytes(joueurs.at(myProto.tempoNum).getName());

					std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> conv2;
					std::wstring myConv2 = conv2.from_bytes(myProto.buff_ + myProto.tempoSize + 5);

					logs.addMessage(myConv + L" : " + myConv2, sf::Color::Blue);
					SoundHandler::playMessage();
				}
				else if(save == 6){
					mapNum.setString(std::to_string(myProto.tempoNum));
					mapName.setString(myProto.tempoBuff);
				}
				else if(save == 7){
					myProto.fillOption(mesOptions);
				}
				else if(save == 42){
					logs.addMessage(L"Début de la partie imminent !", sf::Color::Red);
					map.load(myProto.buff_ + 4);
					phase = 2;
					break;
				}
			}
			if(save == -1) return 0;
		}
		else{
			//Chargement joueur / map avant début de partie
			while((save = myProto.beforeGameRecv2()) > 0){
				if(save == 0){
					continue;
				}
				else if(save == 1){
					std::vector<std::pair<int, std::pair<int, int>>> tempoPlayers;

					myProto.fillAll(tempoPlayers);
					for(size_t i(0), tempoSize(tempoPlayers.size()); i < tempoSize; ++i){
						joueurs.at(tempoPlayers[i].first).setPosition(tempoPlayers[i].second.first, tempoPlayers[i].second.second);
					}
				}
				else if(save == 42){
					start = true;
					break;
				}
			}
			if(save == -1) return 0;
		}
		if(start){
			music.stop();
			int end = partie(app, map, joueurs, myProto);

			if(end == 1) return 1;
			else if(end == 0) return 0;

			music.play();

			map.clear();
			phase = 1;
			start = false;
			test.restart();

			for(auto it(joueurs.begin()), itEnd(joueurs.end()); it != itEnd; ++it){
				it->second.putAlive();
			}
		}


		for(size_t i(0); i < 4; ++i){

			fondecrans[i].move(-1, -1);

			if(fondecrans[i].getPosition().x == -1051*2){
				fondecrans[i].setPosition(1051*2, fondecrans[i].getPosition().y);
			}

		}


		if(fondecrans[0].getPosition().y == -1051*2){
			fondecrans[0].setPosition(fondecran4.getPosition().x, 1051*2);
			fondecrans[1].setPosition(fondecran4.getPosition().x + 1051*2, 1051*2);
		}
		if(fondecrans[2].getPosition().y == -1051*2){
			fondecrans[2].setPosition(fondecran2.getPosition().x, 1051*2);
			fondecrans[3].setPosition(fondecran2.getPosition().x + 1051*2, 1051*2);
		}

		sf::Color actualColor(fabs(cos(test.getElapsedTime().asSeconds())*255), fabs(sin(test.getElapsedTime().asSeconds())*255), fabs(cos(test.getElapsedTime().asSeconds()*0.8)*255));

		app.clear(actualColor);
		
		if(actualColor.r - 40 <= 0) actualColor.r = 0;
		else actualColor.r -= 40;
		if(actualColor.g - 40 <= 0) actualColor.g = 0;
		else actualColor.g -= 40;
		if(actualColor.b - 40 <= 0) actualColor.b = 0;
		else actualColor.b -= 40;

		chatWindow.setFillColor(actualColor);

		for(size_t i(0); i < 4; ++i){
			app.draw(fondecrans[i]);
		}

		logs.draw(app);

		app.draw(chatWindow);
		app.draw(chatText);

		app.draw(startButton);
		app.draw(startText);

		app.draw(clearButton);
		app.draw(clearText);


		//Affichage des informations sur la map actuelle
		app.draw(mapName);
		app.draw(mapText);
		app.draw(mapNum);

		mesOptions.draw(app);

		app.display();
	}

	return 1;

}