#pragma once

#ifndef WALL_HPP
#define WALL_HPP

#include "GameObject.hpp"

class Wall : public GameObject
{
public:
	Wall() : GameObject() {}

	Wall(const Coord& position, fs_path imgPath, int scaleToValue = 64)
		: GameObject(position, imgPath, scaleToValue)
	{}

	void Update(sf::RenderTarget& target) override
	{
		if (!EnsureTextureLoaded())
			return;

		sf::Sprite& spr = AccessSprite();
		spr.setPosition({static_cast<float>(position_.x), static_cast<float>(position_.y)});
		target.draw(spr);
	}
};

#endif // !WALL_HPP
