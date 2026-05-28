#pragma once

#ifndef WINDOW_RENDERER_HPP
#define WINDOW_RENDERER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <map>

#include "GameMap.hpp"
#include "Tile.hpp"
#include "Player.hpp"
#include "Wall.hpp"
#include "Camera.hpp"
#include "Room.hpp"
#include "CollisionChecker.hpp"

// будет хранить карту и рисовать её в окне
class WindowRenderer
{
public:
	WindowRenderer() = default;

	// Создать комнату и добавить в коллекцию
	std::shared_ptr<Room> CreateRoom(int gridX, int gridY)
	{
		auto room = std::make_shared<Room>(gridX, gridY, 1280, 720);
		rooms_[{gridX, gridY}] = room;
		return room;
	}

	// Получить комнату по координатам сетки
	std::shared_ptr<Room> GetRoom(int gridX, int gridY) const
	{
		auto it = rooms_.find({gridX, gridY});
		if (it != rooms_.end())
			return it->second;
		return nullptr;
	}

	void Demo() {
		window_.create(sf::VideoMode({ 1280, 720 }), "Game Map");

		// Создаём камеру (размер карты: 3x3 комнаты по 1280x720 каждая = 3840x2160 пикселей)
		Camera camera(3840, 2160, 1280, 720);

		// Создаём 9 комнат (3x3 сетка)
		for (int gy = 0; gy < 3; ++gy)
		{
			for (int gx = 0; gx < 3; ++gx)
			{
				CreateRoom(gx, gy);
			}
		}

		// Заполняем каждую комнату тайлами и стенами
		for (int gy = 0; gy < 3; ++gy)
		{
			for (int gx = 0; gx < 3; ++gx)
			{
				auto room = GetRoom(gx, gy);
				long roomPosX = room->GetPosX();
				long roomPosY = room->GetPosY();

				// Добавляем тайлы в комнату
				for (int y = 0; y < 11; ++y)
				{
					for (int x = 0; x < 20; ++x)
					{
						Coord tilePos{ roomPosX + x * 64, roomPosY + y * 64 };
						auto tile = std::make_shared<Tile>(tilePos, "./resources/roadTexture_01.png");
						room->AddObject(tile);
					}
				}

				// Добавляем стены границ комнаты
				// Верхняя граница (если это не первый ряд, делаем проход)
				if (gy > 0)
				{
					// Проход в верхнюю комнату (в центре)
					for (int x = 0; x < 20; ++x)
					{
						if (x < 7 || x > 12) // Проход в середине
						{
							auto wall = std::make_shared<Wall>(
								Coord{ roomPosX + x * 64, roomPosY - 64 }, "./resources/wallTexture.png");
							room->AddObject(wall);
						}
					}
				}

				// Нижняя граница (если это не последний ряд, делаем проход)
				if (gy < 2)
				{
					for (int x = 0; x < 20; ++x)
					{
						if (x < 7 || x > 12)
						{
							auto wall = std::make_shared<Wall>(
								Coord{ roomPosX + x * 64, roomPosY + room->GetHeight() }, "./resources/wallTexture.png");
							room->AddObject(wall);
						}
					}
				}

				// Левая граница (если это не первый столбец, делаем проход)
				if (gx > 0)
				{
					for (int y = 0; y < 11; ++y)
					{
						if (y < 4 || y > 6) // Проход в середине
						{
							auto wall = std::make_shared<Wall>(
								Coord{ roomPosX - 64, roomPosY + y * 64 }, "./resources/wallTexture.png");
							room->AddObject(wall);
						}
					}
				}

				// Правая граница (если это не последний столбец, делаем проход)
				if (gx < 2)
				{
					for (int y = 0; y < 11; ++y)
					{
						if (y < 4 || y > 6)
						{
							auto wall = std::make_shared<Wall>(
								Coord{ roomPosX + room->GetWidth(), roomPosY + y * 64 }, "./resources/wallTexture.png");
							room->AddObject(wall);
						}
					}
				}

				// Добавляем несколько внутренних стен для интереса
				if (gx == 0 && gy == 0)
				{
					auto wall1 = std::make_shared<Wall>(
						Coord{ roomPosX + 5 * 64, roomPosY + 3 * 64 }, "./resources/wallTexture.png");
					auto wall2 = std::make_shared<Wall>(
						Coord{ roomPosX + 6 * 64, roomPosY + 3 * 64 }, "./resources/wallTexture.png");
					auto wall3 = std::make_shared<Wall>(
						Coord{ roomPosX + 7 * 64, roomPosY + 3 * 64 }, "./resources/wallTexture.png");
					room->AddObject(wall1);
					room->AddObject(wall2);
					room->AddObject(wall3);
				}
			}
		}

		// Собираем ВСЕ стены из всех комнат в один вектор для проверки коллизий
		std::vector<std::shared_ptr<GameObject>> allWalls;
		for (auto& [key, room] : rooms_)
		{
			for (const auto& obj : room->GetObjects())
			{
				// Добавляем только стены (не тайлы)
				if (dynamic_cast<Wall*>(obj.get()))
					allWalls.push_back(obj);
			}
		}

		// Создаём проверитель коллизий и добавляем туда все стены
		CollisionChecker collisionChecker;
		collisionChecker.AddObstacles(allWalls);

		// Создаём игрока в первой комнате
		auto player = std::make_unique<Player>(
			Coord{ 640, 360 },  // Центр первой комнаты
			"./resources/playerTexture.png"
		);

		// Передаём проверитель коллизий игроку
		player->SetCollisionChecker(&collisionChecker);

		// Переменная для отслеживания текущей комнаты
		int prevRoomX = -1, prevRoomY = -1;

		while (window_.isOpen())
		{
			// Получаем текущую комнату игрока
			int currentRoomX = static_cast<int>(player->GetPosition().x / 1280);
			int currentRoomY = static_cast<int>(player->GetPosition().y / 720);

			// Ограничиваем координаты комнаты границами сетки
			if (currentRoomX < 0) currentRoomX = 0;
			if (currentRoomX > 2) currentRoomX = 2;
			if (currentRoomY < 0) currentRoomY = 0;
			if (currentRoomY > 2) currentRoomY = 2;

			auto currentRoom = GetRoom(currentRoomX, currentRoomY);

			// Обновляем камеру, когда игрок входит в новую комнату
			if (currentRoomX != prevRoomX || currentRoomY != prevRoomY)
			{
				if (currentRoom)
				{
					// Привязываем камеру к центру комнаты
					long roomCenterX = currentRoom->GetPosX() + currentRoom->GetWidth() / 2;
					long roomCenterY = currentRoom->GetPosY() + currentRoom->GetHeight() / 2;
					camera.LookAt(Coord{ roomCenterX, roomCenterY });
					prevRoomX = currentRoomX;
					prevRoomY = currentRoomY;
				}
			}

			window_.clear();
			window_.setView(camera.GetView());

			if (currentRoom)
			{
				// Рисуем объекты текущей комнаты
				for (const auto& obj : currentRoom->GetObjects())
				{
					if (obj)
						obj->Update(window_);
				}
			}

			// Обновляем и рисуем игрока
			player->Update(window_);

			window_.display();
		}
	}
private:
	sf::RenderWindow window_;
	std::map<std::pair<int, int>, std::shared_ptr<Room>> rooms_;
};

#endif // !WINDOW_RENDERER

