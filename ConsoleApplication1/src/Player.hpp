#pragma once

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/System.hpp>
#include <vector>
#include <memory>
#include "GameObject.hpp"
#include "CollisionChecker.hpp"

class Player : public GameObject
{
public:
	Player() : moveDelay_(0.2f), moveClock_(), collisionChecker_(nullptr) {}
	Player(const Coord& position, fs_path imgPath, int scaleToValue = 64)
		: GameObject(position, imgPath, scaleToValue), moveDelay_(0.1f), moveClock_(), collisionChecker_(nullptr)
	{}

	void Update(sf::RenderTarget& target) override
	{
		// Обработка ввода с клавиатуры (стрелки для движения)
		HandleInput();

		// Загружаем текстуру если ещё не загружена
		if (!EnsureTextureLoaded())
			return;

		EnsureTextureLoaded();
		sf::Sprite& spr = AccessSprite();
		// Устанавливаем позицию спрайта в текущие координаты
		spr.setPosition({static_cast<float>(position_.x), static_cast<float>(position_.y)});
		target.draw(spr);
	}

	// Вспомогательный метод для перемещения на указанное расстояние
	void Move(long dx, long dy)
	{
		position_.x += dx;
		position_.y += dy;
	}

	// Устанавливает проверитель коллизий (новый способ)
	void SetCollisionChecker(CollisionChecker* checker)
	{
		collisionChecker_ = checker;
	}

	// Устанавливает список стен для проверки коллизий (DEPRECATED - используйте SetCollisionChecker)
	void SetWalls(std::vector<std::shared_ptr<GameObject>>* walls)
	{
		walls_ = walls;
	}

private:
	float moveDelay_;     // Задержка в секундах между движениями
	sf::Clock moveClock_; // Таймер для отслеживания времени последнего движения
	std::vector<std::shared_ptr<GameObject>>* walls_ = nullptr; // Указатель на список стен (DEPRECATED)
	CollisionChecker* collisionChecker_ = nullptr; // Проверитель коллизий

	// Проверяет, можно ли двигаться (если прошла достаточная задержка)
	bool CanMoveNow()
	{
		if (moveClock_.getElapsedTime().asSeconds() >= moveDelay_)
		{
			moveClock_.restart();
			return true;
		}
		return false;
	}

	// Проверяет коллизию с другим объектом (DEPRECATED - используйте collisionChecker_)
	bool WillCollideWith(const GameObject& other, long newX, long newY) const
	{
		// Границы текущего игрока после перемещения
		long playerLeft = newX;
		long playerRight = newX + scaleToValue_;
		long playerTop = newY;
		long playerBottom = newY + scaleToValue_;

		// Границы другого объекта
		long otherLeft = other.GetPosition().x;
		long otherRight = otherLeft + other.GetScaleToValue();
		long otherTop = other.GetPosition().y;
		long otherBottom = otherTop + other.GetScaleToValue();

		// Проверяем пересечение
		return !(playerRight <= otherLeft || playerLeft >= otherRight || 
				 playerBottom <= otherTop || playerTop >= otherBottom);
	}

	// Обработка нажатий стрелок на клавиатуре
	void HandleInput()
	{
		// Проверяем, прошла ли задержка перед следующим движением
		if (!CanMoveNow())
			return;

		int moveDistance = scaleToValue_; // Движемся на один тайл (64 пикселя по умолчанию)
		long newX = position_.x;
		long newY = position_.y;
		bool moved = false;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
		{
			newY -= moveDistance;
			moved = true;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
		{
			newY += moveDistance;
			moved = true;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		{
			newX -= moveDistance;
			moved = true;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		{
			newX += moveDistance;
			moved = true;
		}

		if (!moved)
			return; // Ни одна клавиша не нажата, не движемся

		// Используем новый CollisionChecker если доступен
		if (collisionChecker_)
		{
			if (collisionChecker_->CanMove(position_, newX - position_.x, newY - position_.y, scaleToValue_, this))
			{
				position_.x = newX;
				position_.y = newY;
			}
			return;
		}



		// Если нет коллизий, двигаемся
		position_.x = newX;
		position_.y = newY;
	}
};

#endif // !PLAYER_HPP
