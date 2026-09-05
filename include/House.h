#ifndef HOUSE_H
#define HOUSE_H

#include <SFML/System.hpp>

class House
{
public:
	House(float x, float y, float width, float height);

	sf::Vector2f getPosition() const;
	float getWidth() const;
	float getHeight() const;

private:
	sf::Vector2f _position;
	float _width;
	float _height;
};

#endif // !HOUSE_H
