#pragma once

#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

//либо строка либо путь сстемы должен применяться для изображения объекта
#include <string>
#include <filesystem>
#include <memory>

#include <SFML/Graphics.hpp>

#include "Coord.hpp"

class GameObject
{
public:

	using fs_path = std::filesystem::path;

	GameObject():
		position_({0, 0}),
		imgPath_(""),
		scaleToValue_(64),
		textureLoaded_(false)
	{
	}

	GameObject(
		const Coord& position, 
		fs_path imgPath, 
		int scaleToValue=64)
		: position_(position),
		imgPath_(std::move(imgPath)),
		scaleToValue_(scaleToValue),
		textureLoaded_(false)
	{}
	virtual ~GameObject() = default;
	
	virtual void Update(sf::RenderTarget& target) = 0;
	
	const Coord& GetPosition() const { return position_; }
	const fs_path& GetImagePath() const { return imgPath_; }
	int GetScaleToValue() const { return scaleToValue_; }
	
	void SetPosition(const Coord& position) { position_ = position; }

	void SetImagePath(const fs_path& imgPath) { imgPath_ = imgPath; textureLoaded_ = false; }	

	void SetScaleToValue(int scaleToValue) 
	{ scaleToValue_ = scaleToValue; }

	bool IsCollidingWith(const GameObject& other) const
	{
		long leftA = position_.x;
		long rightA = position_.x + scaleToValue_;
		long topA = position_.y;
		long bottomA = position_.y + scaleToValue_;

		long leftB = other.position_.x;
		long rightB = other.position_.x + other.scaleToValue_;
		long topB = other.position_.y;
		long bottomB = other.position_.y + other.scaleToValue_;

		return !(leftA >= rightB || rightA <= leftB || topA >= bottomB || bottomA <= topB);
	}

protected:
	// Загружает текстуру при необходимости, настраивает sprite и масштаб.
	bool EnsureTextureLoaded()
	{
		if (textureLoaded_) return true;
		if (imgPath_.empty()) return false;
		// loadFromFile принимает std::string (utf-8) на большинстве платформ
		if (!texture_.loadFromFile(imgPath_.string()))
			return false;

		if (!sprite_)
			sprite_ = std::make_unique<sf::Sprite>(texture_);
		else
			sprite_->setTexture(texture_);

		// Масштабируем спрайт так, чтобы он вписался в квадрат размера scaleToValue_
		auto texSize = texture_.getSize();
		if (texSize.x > 0 && texSize.y > 0)
		{
			float sx = static_cast<float>(scaleToValue_) / static_cast<float>(texSize.x);
			float sy = static_cast<float>(scaleToValue_) / static_cast<float>(texSize.y);
			sprite_->setScale(sf::Vector2f(sx, sy));
		}

		textureLoaded_ = true;
		return true;
	}

	// Доступ для наследников к sprite
	sf::Sprite& AccessSprite() { return *sprite_; }

	int scaleToValue_;
	fs_path imgPath_;	
	Coord position_;

private:
	mutable sf::Texture texture_;
	mutable std::unique_ptr<sf::Sprite> sprite_;
	mutable bool textureLoaded_;
};








#endif // !GAME_OBJECT_HPP

