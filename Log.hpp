#pragma once
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

class Log {

	public:
		explicit Log(int size);
		
		void addMessage(std::wstring const& newMessage, sf::Color const& newCol);

		void clear();

		void draw(sf::RenderWindow& app);

	private:
		std::vector<sf::Text> allMessages;
		int size_;

		sf::Font font_;
};