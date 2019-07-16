#pragma once

#include <SFML/Graphics.hpp>

#include "macro.h"

#include "Player.hpp"
#include "Protocole.hpp"
#include "Carte.hpp"
#include "SoundHandler.hpp"

//loby.cpp
int beforeGoingToLoby(sf::RenderWindow& app, int sock);
int loby(sf::RenderWindow& app, int sock, std::unordered_map<int, Player>& joueurs);

//Partie.cpp
int partie(sf::RenderWindow& app, Carte& map, std::unordered_map<int, Player>& joueurs, Protocole& myProto);

//menu.cpp
int menu(sf::RenderWindow& app);
int menu_ask(sf::RenderWindow& app, sf::String asking_text, std::string& tempo);

//multiplayer.cpp
std::array<std::string, 2> findMyIp();
//int open_tcp_listener(const char *port);
int open_serv(int port);
int serveur(int port);
int connect_to_serv(char const *name, int port);


static std::vector<std::string> FICHIER_SON {"ressources/sounds/bomb.ogg", "ressources/sounds/connect.ogg", "ressources/sounds/disconnect.ogg", "ressources/sounds/message.ogg"};