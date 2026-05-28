#pragma once

#ifndef ROOM_HPP
#define ROOM_HPP

#include <vector>
#include <memory>
#include "Coord.hpp"
#include "GameObject.hpp"

class Room
{
public:
	// x, y - координаты комнаты в сетке комнат (не в пикселях)
	// width, height - размер комнаты в пикселях
	Room(int x, int y, int width = 1280, int height = 720)
		: gridX_(x), gridY_(y), width_(width), height_(height),
		  posX_(x * width), posY_(y * height)
	{}

	int GetGridX() const { return gridX_; }
	int GetGridY() const { return gridY_; }
	long GetPosX() const { return posX_; }
	long GetPosY() const { return posY_; }
	int GetWidth() const { return width_; }
	int GetHeight() const { return height_; }

	// Добавить игровой объект в комнату
	void AddObject(std::shared_ptr<GameObject> obj)
	{
		objects_.push_back(obj);
	}

	// Получить все объекты комнаты
	const std::vector<std::shared_ptr<GameObject>>& GetObjects() const
	{
		return objects_;
	}

	// Проверить, находится ли позиция в этой комнате
	bool ContainsPoint(long x, long y) const
	{
		return x >= posX_ && x < posX_ + width_ &&
			   y >= posY_ && y < posY_ + height_;
	}

	// Получить соседнюю комнату по направлению
	// direction: 0=Up, 1=Down, 2=Left, 3=Right
	bool GetAdjacentRoom(int direction, int& outGridX, int& outGridY) const
	{
		outGridX = gridX_;
		outGridY = gridY_;

		switch (direction)
		{
			case 0: // Up
				outGridY--;
				break;
			case 1: // Down
				outGridY++;
				break;
			case 2: // Left
				outGridX--;
				break;
			case 3: // Right
				outGridX++;
				break;
			default:
				return false;
		}
		return true;
	}

private:
	int gridX_;    // Позиция комнаты в сетке (X)
	int gridY_;    // Позиция комнаты в сетке (Y)
	long posX_;    // Пиксельная позиция комнаты (X)
	long posY_;    // Пиксельная позиция комнаты (Y)
	int width_;    // Ширина комнаты в пикселях
	int height_;   // Высота комнаты в пикселях

	std::vector<std::shared_ptr<GameObject>> objects_;
};

#endif // !ROOM_HPP
