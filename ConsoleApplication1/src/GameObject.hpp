#pragma once

#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

//либо строка либо путь сстемы должен применяться для изображения объекта
#include <string>
#include <filesystem>

#include "Coord.hpp"

class GameObject
{
	using fs_path = std::filesystem::path;

public:
	GameObject():
		position_({0, 0}),
		imgPath_(""),
		scaleToValue_(64)
	{
	}

	GameObject(
		const Coord& position, 
		fs_path imgPath, 
		int scaleToValue=64)
		: position_(position),
		imgPath_(std::move(imgPath)),
		scaleToValue_(scaleToValue)
	{}
	virtual ~GameObject() = default;
	
	virtual void Update() = 0;
	
	const Coord& GetPosition() const { return position_; }
	const fs_path& GetImagePath() const { return imgPath_; }
	int GetScaleToValue() const { return scaleToValue_; }
	
	void SetPosition(const Coord& position) { position_ = position; }

	void SetImagePath(const fs_path& imgPath) { imgPath_ = imgPath; }	

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

	int scaleToValue_;
	fs_path imgPath_;	
	Coord position_;
};








#endif // !GAME_OBJECT_HPP

