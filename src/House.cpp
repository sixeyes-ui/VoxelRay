#include "include/House.h"

House::House(float x, float y, float width, float height)
	: _position(x, y),
	  _width(width),
	  _height(height)
{
}

sf::Vector2f House::getPosition() const
{
	return _position;
}

float House::getWidth() const
{
	return _width;
}

float House::getHeight() const
{
	return _height;
}