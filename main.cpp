#include "head.h"
#include <signal.h>

int main(int argc, char const *argv[]){

	srand(time(NULL));
	
	int sock;
	std::string tempo, tempo2, tempo3;

	if(argc == 2){
		int port;
		try {
		    port = std::stoi(argv[1]);
        } catch (
        	const std::invalid_argument& ia) {
          	std::cerr << "Port invalide : " << ia.what() << '\n';
          	return 1;
        }

        int srv = open_serv(port);

        if(srv < 0){
			fprintf(stderr, "Could not listen\n.");
			return 0;
		}

		return serveur(srv);
	}
 	
	//Preparartion pour le SoundHandler
	SoundHandler::initialiserSons(FICHIER_SON);

	sf::RenderWindow app(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 64), "Bomberman By Magnat");
	app.setVerticalSyncEnabled(true);

	//Musique du menu
	sf::Music menuMusic;
	menuMusic.openFromFile("ressources/music/menu.ogg");
	menuMusic.play();
	
	while(app.isOpen()){
		int save(menu(app));

		if(save == 1){
			tempo.clear();
			save = menu_ask(app, L"Veuillez rentrer l'adresse du serveur :", tempo);
			if(save != 1) continue;

			tempo2.clear();
			save = menu_ask(app, L"Veuillez rentrer le port du serveur :", tempo2);
			if(save != 1) continue;

			try {
			    save = std::stoi(tempo2.data());
            } catch (
            	const std::invalid_argument& ia) {
              	std::cerr << "Port invalide : " << ia.what() << '\n';
              	continue;
            }

            tempo3.clear();
			if(menu_ask(app, L"Choisissez un Pseudo :", tempo3) != 1) continue;

			sock = connect_to_serv(tempo.data(), save);

			if(sock < 0) continue;

			std::string logPseudo;
			if(tempo3.empty()){
				logPseudo = "LOG Player\n";
			}
			else{
				logPseudo = "LOG " + tempo3 + "\n";
			}
			menuMusic.stop();
			send(sock, logPseudo.data(), logPseudo.size(), 0);
			int end = beforeGoingToLoby(app, sock);

			close(sock);

			if(end == 1) break;

			menuMusic.play();
		}
		else if(save == 2){
			save = menu_ask(app, L"Veuillez rentrer le port du serveur à créer :", tempo);
			if(save != 1) continue;

			try {
			    save = std::stoi(tempo.data());
            } catch (
            	const std::invalid_argument& ia) {
              	std::cerr << "Port invalide : " << ia.what() << '\n';
              	continue;
            }

			if(menu_ask(app, L"Choisissez un Pseudo :", tempo3) != 1) continue;

			std::string logPseudo;
			if(tempo3.empty()){
				logPseudo = "LOG Player\n";
			}
			else{
				logPseudo = "LOG " + tempo3 + "\n";
			}

			int srv = open_serv(save);
			if(srv < 0){
				fprintf(stderr, "Could not listen\n.");
				continue;
			}
			int pid = fork();
			if(pid == 0){
				serveur(srv);
			}
			else{
				sock = connect_to_serv("localhost", save);

				if(sock == -1){
					kill(pid, SIGTERM);
					close(srv);
					close(sock);
					continue;
				}

				menuMusic.stop();
				send(sock, logPseudo.data(), logPseudo.size(), 0);
				int end = beforeGoingToLoby(app, sock);
				
				kill(pid, SIGTERM);
				close(srv);
				close(sock);

				if(end == 1) break;

				menuMusic.play();
			}
		}
	}
	return 0;
}