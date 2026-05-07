#pragma once

#ifndef GAME_MAP_HPP
#define GAME_MAP_HPP

#include <vector>
#include <memory>

#include "Coord.hpp"
#include "GameObject.hpp"
//20x11 клеток, 1280x720 пикселей, 64 пикселя на клетку
class GameMap
{
public:
	GameMap() {
		gameObjects.resize(11); // 11 строк
		for (auto& row : gameObjects) {
			row.resize(20); // 20 столбцов
		}
	}
	
	std::vector <std::unique_ptr<GameObject>>& operator[](int n) {
		return gameObjects[n];
	}

private:
	std::vector<std::vector<std::unique_ptr<GameObject>>> gameObjects;//матрица клеток

};



#endif
