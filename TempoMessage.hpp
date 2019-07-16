#pragma once

#include <SFML/Graphics.hpp>
#include <string>


class TempoMessage {
public:

	TempoMessage();

	void setNewMessage(std::wstring const& message, sf::Vector2f const& position, float timeAliveMilli);

	void draw(sf::RenderWindow& app);

private:

	bool actif_;
	bool full_;

	float transparence_;

	float aliveTimerMilli_;
	sf::Clock aliveClock_;

	sf::Font font_;
	sf::Text message_;
};