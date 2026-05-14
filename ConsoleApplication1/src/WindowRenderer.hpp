#pragma once

#ifndef WINDOW_RENDERER_HPP
#define WINDOW_RENDERER_HPP

#include <SFML/Graphics.hpp>

#include "GameMap.hpp"
#include "Tile.hpp"
// будет хранить карту и рисовать её в окне
class WindowRenderer
{
public:
	WindowRenderer() = default;
	void Demo() {
		GameMap map;
		window_.create(sf::VideoMode({ 1280, 720 }), "Game Map");
		map[0][0] = std::make_unique<Tile>(Coord{ 0, 0 }, "./resources/roadTexture_01.png");
		map[0][1] = std::make_unique<Tile>(Coord{ 128, 0 }, "./resources/roadTexture_01.png");
		while (window_.isOpen())
		 {
			 window_.clear();
			 map[0][0]->Update(window_);
			 map[0][1]->Update(window_);
			 window_.display();
		}

	}
private:
	sf::RenderWindow window_;


};



#endif // !WINDOW_RENDERER
