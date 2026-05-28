#pragma once

#ifndef COLLISION_CHECKER_HPP
#define COLLISION_CHECKER_HPP

#include <vector>
#include <memory>
#include "GameObject.hpp"

class CollisionChecker
{
public:
	CollisionChecker() = default;

	// Добавить препятствие для проверки коллизий
	void AddObstacle(std::shared_ptr<GameObject> obstacle)
	{
		obstacles_.push_back(obstacle);
	}

	// Добавить несколько препятствий сразу
	void AddObstacles(const std::vector<std::shared_ptr<GameObject>>& obstacles)
	{
		for (const auto& obstacle : obstacles)
		{
			obstacles_.push_back(obstacle);
		}
	}

	// Очистить список препятствий
	void ClearObstacles()
	{
		obstacles_.clear();
	}

	// Проверить, столкнётся ли объект при движении
	// position - текущая позиция
	// newX, newY - новая позиция
	// size - размер объекта (обычно 64)
	// excludeObject - указатель на объект, который нужно исключить из проверки (сам объект)
	bool WillCollide(const Coord& position, long newX, long newY, int size, const GameObject* excludeObject = nullptr) const
	{
		// Границы объекта после перемещения
		long objLeft = newX;
		long objRight = newX + size;
		long objTop = newY;
		long objBottom = newY + size;

		// Проверяем коллизию с каждым препятствием
		for (const auto& obstacle : obstacles_)
		{
			if (!obstacle)
				continue;

			// Исключаем сам объект из проверки
			if (obstacle.get() == excludeObject)
				continue;

			const Coord& obstPos = obstacle->GetPosition();
			int obstSize = obstacle->GetScaleToValue();

			// Границы препятствия
			long obstLeft = obstPos.x;
			long obstRight = obstLeft + obstSize;
			long obstTop = obstPos.y;
			long obstBottom = obstTop + obstSize;
			
			// Проверяем пересечение AABB (Axis-Aligned Bounding Box)
			if (!(objRight <= obstLeft || objLeft >= obstRight || 
				  objBottom <= obstTop || objTop >= obstBottom))
			{
				return true; // Есть коллизия
			}
		}

		return false; // Коллизии нет
	}

	// Проверить, может ли объект двигаться в направлении
	// Возвращает true, если движение возможно
	bool CanMove(const Coord& position, long dx, long dy, int size, const GameObject* excludeObject = nullptr) const
	{
		long newX = position.x + dx;
		long newY = position.y + dy;
		return !WillCollide(position, newX, newY, size, excludeObject);
	}

	// Получить количество препятствий
	size_t GetObstacleCount() const
	{
		return obstacles_.size();
	}

private:
	std::vector<std::shared_ptr<GameObject>> obstacles_;
};

#endif // !COLLISION_CHECKER_HPP
