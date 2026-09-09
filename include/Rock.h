#ifndef ROCK_H
#define ROCK_H

#include <SFML/System.hpp>

class Rock
{
public:
	Rock(float x, float y);

	sf::Vector2f getPosition() const { return _position; }

private:
	sf::Vector2f _position;
};

#endif // !ROCK_H
