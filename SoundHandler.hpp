#pragma once

#include <SFML/Audio.hpp>

#include <vector>
#include <string>
#include <iostream>



class SoundHandler {

	public:
		static void initialiserSons(std::vector<std::string> fichiers);

		/*static void playDeath(){
			findLecteur(rand()%9+10);
		}*/

		static void playBomb(){
			findLecteur(0);
		}

		static void playConnect(){
			findLecteur(1);
		}

		static void playDisconnect(){
			findLecteur(2);
		}

		static void playMessage(){
			findLecteur(3);
		}

	private:
		static void findLecteur(int indexAjouer);



	public:
		//0 - 1 gold, 2 - 6 smoke, 7 - 9 swing,
		static std::vector<sf::SoundBuffer> sons_;
		static std::vector<sf::Sound> lecteur_;

};