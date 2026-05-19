#pragma once

#ifndef WALL_HPP
#define WALL_HPP

#include "GameObject.hpp"

class Wall : public GameObject
{
public:
	Wall() : color_(sf::Color::Black) {}
	
	Wall(const Coord& position, int scaleToValue = 64, const sf::Color& color = sf::Color::Black)
		: GameObject(position, "", scaleToValue), color_(color)
	{}

	void Update(sf::RenderTarget& target) override
	{
		// Рисуем стену как цветной квадрат
		sf::RectangleShape rect(sf::Vector2f(static_cast<float>(scaleToValue_), static_cast<float>(scaleToValue_)));
		rect.setPosition(sf::Vector2f(static_cast<float>(position_.x), static_cast<float>(position_.y)));
		rect.setFillColor(color_);
		target.draw(rect);
	}

	void SetColor(const sf::Color& color) { color_ = color; }
	const sf::Color& GetColor() const { return color_; }

private:
	sf::Color color_;
};

#endif // !WALL_HPP
