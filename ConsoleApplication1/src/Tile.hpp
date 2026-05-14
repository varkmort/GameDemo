#pragma once

#ifndef TILE_HPP
#define TILE_HPP

#include <SFML/Graphics.hpp>

#include "GameObject.hpp"

class Tile : public GameObject
{
public:
	Tile() = default;
	Tile(const Coord& position, fs_path imgPath, int scaleToValue = 64)
		:GameObject(position, imgPath, scaleToValue),
		texture_(imgPath)
	{}

	void Update(sf::RenderTarget& target) override
	{
		//1 убедится что можно загрузить текстуру
		if (!IsTextureLoaded())
		{
			return;
		}

		//2 подготовили позицию и спрайт
		sf::Sprite sprite_(texture_);
		sprite_.setPosition({
			static_cast<float>(position_.x),
			static_cast<float>(position_.y) });

		//3 нарисовали
		target.draw(sprite_);
	}

private:

	bool IsTextureLoaded() const 
	{ 
		return texture_.getSize().x > 0 && texture_.getSize().y > 0; 
	}

	sf::Texture texture_;

};




#endif // !TILE_HPP

