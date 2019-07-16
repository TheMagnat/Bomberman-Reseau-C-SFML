
#include "multiplayer.h"
#include "head.h"
#include "macro.h"

#include <algorithm>
#include <unordered_map>
#include <vector>
#include <utility>
#include <chrono>

#include <experimental/filesystem>

#include "ProtocoleServer.hpp"

//Inspiré d'exmple sur internet et un peu modifié, pas sur du bon fonctionnement de celle-ci sur d'autres machines
std::array<std::string, 2> findMyIp() {
    struct ifaddrs * ifAddrStruct=NULL;
    struct ifaddrs * ifa=NULL;
    void * tmpAddrPtr=NULL;

    std::array<std::string, 2> ret;

    getifaddrs(&ifAddrStruct);

    int i;

    for(ifa = ifAddrStruct, i = 0; ifa != NULL; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr){
            continue;
        }
        if (ifa->ifa_addr->sa_family == AF_INET) {
            ++i;

            if(i != 2 && i != 4) continue;

            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);

            if(i == 2) ret[0] = addressBuffer;
            if(i == 4) ret[1] = addressBuffer;

        } else if (ifa->ifa_addr->sa_family == AF_INET6) {
            ++i;

            if(i != 2 && i != 4) continue;

            tmpAddrPtr=&((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
            char addressBuffer[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);

            if(i == 2) ret[0] = addressBuffer;
            if(i == 4) ret[1] = addressBuffer;
        } 
    }

    if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);

    return ret;
}


//Fonction du cours pour ouvrir sur toute les ip
//Est totallement fonctionnelle mais n'est pas utilisé
/*
int open_tcp_listener(const char *port){

	struct addrinfo hints, *addrinfo2, *ai;
	int n, srv, ret;
	char ipstr[INET6_ADDRSTRLEN];

	bzero(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((n = getaddrinfo(NULL, port, &hints, &addrinfo2)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(n));
		exit(1);
	}

	for (ai = addrinfo2; ai != nullptr; ai = ai->ai_next) {
		switch (ai->ai_family) {
			case AF_INET:
				inet_ntop(ai->ai_family, &((struct sockaddr_in *)ai->ai_addr)->sin_addr,
				ipstr, sizeof(ipstr));
				break;
			case AF_INET6:
				inet_ntop(ai->ai_family, &((struct sockaddr_in6 *)ai->ai_addr)->sin6_addr,
				ipstr, sizeof(ipstr));
				break;
			default: continue;
		}
		printf("Trying to listen on %s on port %s…\n", ipstr, port);

		if ((srv = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol)) < 0) {
			perror("socket");
		}
		int test(1);
		if (setsockopt(srv, SOL_SOCKET, SO_REUSEADDR, &test, sizeof(int)) < 0) {
			perror("setsockopt");
			exit(1);
		}
		if (fcntl(srv, F_SETFL, O_NONBLOCK) < 0) {
			perror("fcntl");
			exit(1);
		}
		if (bind(srv, ai->ai_addr, ai->ai_addrlen) < 0) {
			perror("bind");
			exit(1);
		}
		if (listen(srv, 8) < 0) {
			perror("listen");
			exit(1);
		}

		ret = srv;
		
		printf("Listening on %s on port %s\n", ipstr, port);

		break;
	}

	freeaddrinfo(addrinfo2);

	return ret;
}
*/

//Fonction pour la création de la data de MAP
void fileToString(std::string const& file, std::string& stringBuf, const char* header, int sizeHead){
	int tempo;
	stringBuf.clear();
	std::ifstream fichier(file, std::ios::in);

	for(int i(0); i < sizeHead; ++i){
		stringBuf.push_back(header[i]);
	}

	fichier >> tempo;
	stringBuf += std::to_string(tempo);
	stringBuf.push_back(' ');
	fichier >> tempo;
	stringBuf += std::to_string(tempo);
	while(fichier.good()){
		stringBuf.push_back(' ');
		fichier >> tempo;
		stringBuf.push_back(tempo + '0');
	}

	fichier.close();
}

//Fonction pour la créeation de la data de ALL
void stringAllPos(std::string const& file, std::string& stringBuf, std::unordered_map<int, std::pair<size_t, std::string>> const& fdToPlayer){
	int tempo;
	size_t nbPlayer(fdToPlayer.size());

	stringBuf.clear();
	std::ifstream fichier2(file, std::ios::in);

	stringBuf += "ALL ";
	stringBuf += std::to_string(nbPlayer);

	for(auto it(fdToPlayer.begin()), itEnd(fdToPlayer.end()); it != itEnd; ++it){
		stringBuf.push_back(' ');

		//Numero
		stringBuf += std::to_string(it->second.first);
		stringBuf.push_back(' ');

		//Position
		fichier2 >> tempo;
		stringBuf += std::to_string(tempo);
		stringBuf.push_back(' ');
		fichier2 >> tempo;
		stringBuf += std::to_string(tempo);
	}

	stringBuf.push_back('\n');
}

//Fonction pour la création de la data de LST
void stringLst(std::string& stringBuf, std::unordered_map<int, std::pair<size_t, std::string>> const& fdToPlayer){

	size_t nbPlayer(0);

	stringBuf.clear();

	//Laisse un trou à l'emplacement 4
	stringBuf += "LST ";
	
	//stringBuf += std::to_string(nbPlayer);

	for(auto it(fdToPlayer.begin()), itEnd(fdToPlayer.end()); it != itEnd; ++it){

		if(it->second.second.empty()) continue;

		++nbPlayer;

		stringBuf.push_back(' ');

		//Numero
		stringBuf += std::to_string(it->second.first);
		stringBuf.push_back(' ');

		//Pseudo
		stringBuf += it->second.second;
	}

	stringBuf.insert(4, std::to_string(nbPlayer));

	stringBuf.push_back('\n');

}

//Fonction pour ouvrir une socket serveur
int open_serv(int port){

	int srv, opt(1);
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;

	if((srv = socket(AF_INET , SOCK_STREAM, 0)) < 0){
		perror("socket");
		return -1;
	}
	if(setsockopt(srv, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0){
		perror("setsockopt");
		return -1;
	}
	if(fcntl(srv, F_SETFL, O_NONBLOCK) < 0){
		perror("fcntl");
		return -1;
	}
	if(bind(srv, (struct sockaddr *) &addr, sizeof(addr)) < 0){
		perror("bind");
		return -1;
	}
	if(listen(srv, 8) < 0){
		perror("listen");
		return -1;
	}

	return srv;
}

//Fonction Principale du serveur
int serveur(int srv){
	int fd;

	std::unordered_map<int, std::pair<size_t, std::string>> fdToPlayer;

	size_t loggedClient(0), hote(0);
	int len, save;
	int etat(0);
	char buf[BUFFER_SIZE];
	std::string stringBuf;

	//Charge l'emplacement des maps

	std::vector<std::string> allMap;

    std::experimental::filesystem::directory_iterator myIte("ressources/map");
    for(auto it = std::experimental::filesystem::begin(myIte), itEnd = std::experimental::filesystem::end(myIte); it != itEnd; ++it)
        allMap.emplace_back(std::experimental::filesystem::path(it->path()).replace_extension(""));

    //On les trie puis on enleve les copy consecutive
	std::sort(allMap.begin(), allMap.end());
    auto test = std::unique(allMap.begin(), allMap.end());

    allMap.resize(std::distance(allMap.begin(), test));

    //Le nombre de map
    ssize_t nbMap(allMap.size());
    //Map actuellement choisi
    ssize_t actualMap(0);

    std::vector<std::string> allMapName;
    for(ssize_t i(0); i < nbMap; ++i){
    	allMapName.push_back(std::experimental::filesystem::path(allMap[i]).filename());
    }


	//Pour savoir les troue (Quand une client deco et laisse un trou dans les numero)
	std::vector<size_t> disponible;

	//Gestion des requetes
	ProtocoleServer myProtoServ(&etat, &hote);

	//Pollfd
	std::vector<pollfd> fds;
	fds.emplace_back();
	fds[0].fd = srv;
	fds[0].events = POLLIN;

	for (;;) {
		size_t nfds(fds.size());
		int n(poll(fds.data(), nfds, 300000));
		if (n < 0) {
			perror("poll");
			printf("erreur\n");
			continue;
		}
		else if (n == 0) { // timeout 
		  	continue;
		}
		if (fds[0].revents & POLLIN) { //Nouveau client qui tente de se connecter
			if((fd = ::accept(srv, (struct sockaddr *) NULL, NULL)) < 0){
				if (errno == EWOULDBLOCK) {
					fprintf(stderr, "accept EWOULDBLOCK");
				}
				continue;
			}
			/*if(nfds == MAX_CLIENTS) { //est rejeté si le serveur est plein, pas utilise depuis le passage en vecteur de fds
				write(fd, "ERR Server is Full\n", 19);
				close(fd);
				continue;
			}*/
			if(etat != 0){ //Si la partie n'est pas au loby, refuse la connexion 
				write(fd, "ERR In Game\n", 12);
				close(fd);
				continue;
			}
			fds.emplace_back();
			fds.back().fd = fd;
			fds.back().events = POLLIN;
			if(disponible.empty()){
				stringBuf = std::string("CND ") + std::to_string(nfds-1) + std::string("\n");
				fdToPlayer[fd] = {nfds-1, std::string()};
			}
			else{ //Si un trou présent, le remplis
				stringBuf = std::string("CND ") + std::to_string(disponible.back()) + std::string("\n");
				fdToPlayer[fd] = {disponible.back(), std::string()};
				disponible.pop_back();
			}
			send(fd, stringBuf.data(), stringBuf.size(), 0);
			++nfds;

			std::cout << "Nouveau client numéro " << fdToPlayer[fd].first << " non connecté\n";

		}
		for (size_t i(1); i < nfds; i++) {

			if ((fds[i].revents & POLLIN) == 0) continue;
			bzero(buf, sizeof(buf));

			if ((len = read(fds[i].fd, buf, sizeof(buf))) < 0) {
				perror("read");
			}
			else if(len == 0){ // déconnexion 1
				disponible.push_back(fdToPlayer[fds[i].fd].first);

				if(!fdToPlayer[fds[i].fd].second.empty()){
					--loggedClient;
				}

				if(etat != 0){
					myProtoServ.disconnect(fdToPlayer[fds[i].fd].first);
				}

				std::string goodBye("LFT " + std::to_string(disponible.back()) + "\n");

				std::cout << "Client numéro " << disponible.back() << " à quité le serveur\n";

				bool hoteLeft(false);
				if(fdToPlayer[fds[i].fd].first == hote){
					hoteLeft = true;
				}

				fdToPlayer.erase(fds[i].fd);

				if(hoteLeft){
					if(fdToPlayer.empty()) hote = 0;
					else hote = fdToPlayer.begin()->second.first;
				}

				//Pour ne pas bloquer le serveur en jeu
				if(etat != 0 && fdToPlayer.empty()){
					etat = 0;
				}

				close(fds[i].fd);

				fds[i]=fds.back();
				fds.pop_back();

				--nfds;
				--i;

				for(size_t j(1); j < nfds; ++j){
					send(fds[j].fd, goodBye.data(), goodBye.size(), 0);
				}

			}
			else{ //Message recu
				if(etat == 0){
					save = myProtoServ.beforeGameRecv(buf, fdToPlayer[fds[i].fd].first);
					if(save == 1){
						if(!fdToPlayer[fds[i].fd].second.empty()) continue;

						if(myProtoServ.tempoBuff.empty()) continue;

						++loggedClient;

						stringLst(stringBuf, fdToPlayer);

						//Envoie de LST au nouveau client log
						send(fds[i].fd, stringBuf.data(), stringBuf.size(), 0);
						
						fdToPlayer[fds[i].fd].second = myProtoServ.tempoBuff;

						std::cout << "Client numéro " << fdToPlayer[fds[i].fd].first << " est connecté sous le pseudo " << fdToPlayer[fds[i].fd].second << std::endl;

						for(size_t j(1); j < nfds; ++j){
							send(fds[j].fd, myProtoServ.sendBuff.data(), myProtoServ.sendBuff.size(), 0);
						}

						myProtoServ.FillCfg();

						send(fds[i].fd, myProtoServ.sendBuff.data(), myProtoServ.sendBuff.size(), 0);

						stringBuf = "ACT " + std::to_string(actualMap) + " " + allMapName[actualMap] + "\n";

						send(fds[i].fd, stringBuf.data(), stringBuf.size(), 0);

					}
					else if(save == 2){
						for(size_t j(1); j < nfds; ++j){
							send(fds[j].fd, myProtoServ.sendBuff.data(), myProtoServ.sendBuff.size(), 0);
						}
					}
					else if(save == 3){
						++actualMap;
						if(actualMap == nbMap) actualMap = 0;
						stringBuf = "ACT " + std::to_string(actualMap) + " " + allMapName[actualMap] + "\n";
						for(size_t j(1); j < nfds; ++j){
							send(fds[j].fd, stringBuf.data(), stringBuf.size(), 0);
						}
					}
					else if(save == 4){
						--actualMap;
						if(actualMap < 0) actualMap = nbMap-1;
						stringBuf = "ACT " + std::to_string(actualMap) + " " + allMapName[actualMap] + "\n";
						for(size_t j(1); j < nfds; ++j){
							send(fds[j].fd, stringBuf.data(), stringBuf.size(), 0);
						}
					}
					else if(save == 69){ // déconnexion 2, presque inutile

						disponible.push_back(fdToPlayer[fds[i].fd].first);

						std::cout << "Client numéro " << disponible.back() << " à quité le serveur\n";

						if(!fdToPlayer[fds[i].fd].second.empty()){
							--loggedClient;
						}

						bool hoteLeft(false);
						if(fdToPlayer[fds[i].fd].first == hote){
							hoteLeft = true;
						}

						fdToPlayer.erase(fds[i].fd);

						if(hoteLeft){
							if(fdToPlayer.empty()) hote = 0;
							else hote = fdToPlayer.begin()->second.first;
						}

						//Pour ne pas bloquer le serveur en jeu
						if(etat != 0 && fdToPlayer.empty()){
							etat = 0;
						}

						close(fds[i].fd);

						fds[i]=fds.back();
						fds.pop_back();

						--nfds;
						--i;

						for(size_t j(1); j < nfds; ++j){
							send(fds[j].fd, myProtoServ.sendBuff.data(), myProtoServ.sendBuff.size(), 0);
						}

					}
					else if(save == 70){

						if(loggedClient < 2) continue;

						//Avant tout, on kick les joueurs non log.
						for(auto it(fdToPlayer.begin()), itEnd(fdToPlayer.end()); it != itEnd; ){
							if(!it->second.second.empty()){
								++it;
								continue;
							}

							int toDelFd(it->first);

							it = fdToPlayer.erase(it);

							for(size_t j(1), taille(fds.size()); j < taille; ++j){
								if(fds[j].fd != toDelFd) continue;
								fds[j] = fds.back();
								fds.pop_back();
								close(toDelFd);
								break;
							}

						}


						fileToString(allMap[actualMap]+".txt", stringBuf, "MAP ", 4);
						stringBuf.push_back('\n');

						for(size_t j(1); j < nfds; ++j){
							send(fds[j].fd, stringBuf.data(), stringBuf.size(), 0);
						}

						myProtoServ.FillMap(stringBuf);

						stringAllPos(allMap[actualMap]+".txp", stringBuf, fdToPlayer);
						for(size_t j(1); j < nfds; ++j){
							send(fds[j].fd, stringBuf.data(), stringBuf.size(), 0);
						}
						myProtoServ.FillPlayers(stringBuf);
						myProtoServ.FillPlayersFd(fds);
						
						stringBuf = "STR 2\n";
						for(size_t j(1); j < nfds; ++j){
							send(fds[j].fd, stringBuf.data(), stringBuf.size(), 0);
						}

						std::cout << "Début de partie dans 2 secondes...\n"; 

						std::this_thread::sleep_for(std::chrono::seconds(2));

						std::cout << "Début de partie !\n";

						etat = 1;
					}
				}
				else if(etat == 1){
					save = myProtoServ.duringGameRecv(buf, fdToPlayer[fds[i].fd].first);
					if(save == 1){
						for(size_t j(1); j < nfds; ++j){
							send(fds[j].fd, myProtoServ.sendBuff.data(), myProtoServ.sendBuff.size(), 0);
						}
					}
				}
				else if(etat == 2){
					save = myProtoServ.afterGameRecv(buf, fdToPlayer[fds[i].fd].first);
					if(save == 1){
						for(size_t j(1); j < nfds; ++j){
							send(fds[j].fd, myProtoServ.sendBuff.data(), myProtoServ.sendBuff.size(), 0);
						}

						std::cout << "Retour au lobby\n";

						etat = 0;
					}
				}
			}

		}
	}

	close(srv);

	return 0;
}

//Client
//Pour ouvrir une socket entre notre client et le serveur
int connect_to_serv(char const *name, int port){

	int sock;
	struct hostent *host;
	struct sockaddr_in addr;

	host = gethostbyname(name);
	if(host == NULL){
		perror("gethostbyname");
		return -1;
	}

	sock = socket(AF_INET , SOCK_STREAM, 0);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = *((uint32_t *) host->h_addr);

	if(connect(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0){
		perror("connect");
		return -1;
	}

	return sock;
}
