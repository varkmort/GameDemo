#pragma once

#ifndef WINDOW_RENDERER_HPP
#define WINDOW_RENDERER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include "GameMap.hpp"
#include "Tile.hpp"
#include "Player.hpp"
#include "Wall.hpp"

// будет хранить карту и рисовать её в окне
class WindowRenderer
{
public:
	WindowRenderer() = default;
	void Demo() {
		GameMap map;
		window_.create(sf::VideoMode({ 1280, 720 }), "Game Map");

		// Создаём игровое поле из тайлов (20x11)
		for (int y = 0; y < 11; ++y)
		{
			for (int x = 0; x < 20; ++x)
			{
				Coord tilePos{ x * 64, y * 64 };
				map[y][x] = std::make_unique<Tile>(tilePos, "./resources/roadTexture_01.png");
			}
		}

		// Создаём список стен
		std::vector<std::shared_ptr<GameObject>> walls;

		// Добавляем стены вокруг области (границы поля)
		for (int x = 0; x < 20; ++x)
		{
			// Верхняя стена
			walls.push_back(std::make_shared<Wall>(Coord{ x * 64, -64 }, 64, sf::Color::Red));
			// Нижняя стена
			walls.push_back(std::make_shared<Wall>(Coord{ x * 64, 11 * 64 }, 64, sf::Color::Red));
		}

		for (int y = 0; y < 11; ++y)
		{
			// Левая стена
			walls.push_back(std::make_shared<Wall>(Coord{ -64, y * 64 }, 64, sf::Color::Red));
			// Правая стена
			walls.push_back(std::make_shared<Wall>(Coord{ 20 * 64, y * 64 }, 64, sf::Color::Red));
		}

		// Добавляем несколько стен внутри поля (примеры)
		walls.push_back(std::make_shared<Wall>(Coord{ 5 * 64, 3 * 64 }, 64, sf::Color::Blue));
		walls.push_back(std::make_shared<Wall>(Coord{ 6 * 64, 3 * 64 }, 64, sf::Color::Blue));
		walls.push_back(std::make_shared<Wall>(Coord{ 7 * 64, 3 * 64 }, 64, sf::Color::Blue));
		walls.push_back(std::make_shared<Wall>(Coord{ 10 * 64, 5 * 64 }, 64, sf::Color::Blue));
		walls.push_back(std::make_shared<Wall>(Coord{ 10 * 64, 6 * 64 }, 64, sf::Color::Blue));

		// Создаём игрока в центре поля
		auto player = std::make_unique<Player>(
			Coord{ 5 * 64, 5 * 64 },  // Позиция в центре примерно
			"./resources/playerTexture.png"  // Путь к текстуре игрока 
		);

		// Передаём список стен игроку
		player->SetWalls(&walls);

		while (window_.isOpen())
		{
			window_.clear();

			// Рисуем все тайлы
			for (int y = 0; y < 11; ++y)
			{
				for (int x = 0; x < 20; ++x)
				{
					if (map[y][x])
						map[y][x]->Update(window_);
				}
			}

			// Рисуем все стены
			for (auto& wall : walls)
			{
				if (wall)
					wall->Update(window_);
			}

			// Обновляем и рисуем игрока
			player->Update(window_);

			window_.display();
		}
	}
private:
	sf::RenderWindow window_;


};



#endif // !WINDOW_RENDERER

