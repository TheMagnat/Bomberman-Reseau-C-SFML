#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "macro.h"

class Case {
	public:
		Case(sf::Vector2f const& pos, sf::IntRect const& textureRect, int type = 1);

	protected:
		int type_;

		sf::Vector2f pos_;
		sf::FloatRect hitBox_;
		sf::IntRect textureRect_;
};

class Block : public Case {

	public:
		Block(sf::Texture* const& texture, sf::Vector2f const& pos, sf::IntRect const& textureRect, int const& type = 2);

		sf::Sprite& getSprite();

	private:
		sf::Sprite sprite_;

};