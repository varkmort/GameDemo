#pragma once

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <SFML/Graphics.hpp>
#include "Coord.hpp"

class Camera
{
public:
	// worldWidth и worldHeight - размер всей карты в пикселях
	// viewWidth и viewHeight - размер видимой области (обычно размер окна)
	Camera(int worldWidth, int worldHeight, int viewWidth = 1280, int viewHeight = 720)
		: worldWidth_(worldWidth), worldHeight_(worldHeight),
		  viewWidth_(viewWidth), viewHeight_(viewHeight),
		  posX_(0), posY_(0)
	{
		view_.setSize(sf::Vector2f(static_cast<float>(viewWidth), static_cast<float>(viewHeight)));
		UpdateViewPosition();
	}

	// Переместить камеру на позицию (привязка к объекту)
	void LookAt(const Coord& position)
	{
		// Центрируем камеру на позицию
		posX_ = position.x + 32 - viewWidth_ / 2;  // 32 = scaleToValue/2
		posY_ = position.y + 32 - viewHeight_ / 2;

		// Ограничиваем камеру границами карты
		if (posX_ < 0) posX_ = 0;
		if (posY_ < 0) posY_ = 0;
		if (posX_ + viewWidth_ > worldWidth_)
			posX_ = worldWidth_ - viewWidth_;
		if (posY_ + viewHeight_ > worldHeight_)
			posY_ = worldHeight_ - viewHeight_;

		UpdateViewPosition();
	}

	// Получить sf::View для установки в окно
	const sf::View& GetView() const { return view_; }

	// Получить текущую позицию камеры (верхний левый угол видимой области)
	long GetX() const { return posX_; }
	long GetY() const { return posY_; }

	// Проверить, видна ли точка в текущей области видимости
	bool IsVisible(const Coord& position, int size = 64) const
	{
		long left = posX_;
		long right = posX_ + viewWidth_;
		long top = posY_;
		long bottom = posY_ + viewHeight_;

		long objRight = position.x + size;
		long objBottom = position.y + size;

		return !(objRight < left || position.x > right || 
				 objBottom < top || position.y > bottom);
	}

private:
	sf::View view_;
	int worldWidth_;   // Ширина всей карты в пикселях
	int worldHeight_;  // Высота всей карты в пикселях
	int viewWidth_;    // Ширина видимой области
	int viewHeight_;   // Высота видимой области
	long posX_;        // Текущая позиция камеры X
	long posY_;        // Текущая позиция камеры Y

	void UpdateViewPosition()
	{
		view_.setCenter(sf::Vector2f(posX_ + viewWidth_ / 2.0f, posY_ + viewHeight_ / 2.0f));
	}
};

#endif // !CAMERA_HPP
