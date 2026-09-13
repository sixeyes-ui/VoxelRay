#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/System.hpp>
#include "Animation.h"

class Player
{
public:
	Player(float x, float y);

	void update(float deltaTime, float cameraAngle);

	sf::Vector2f getPosition() const;
	float getAngle() const;

private:
	bool canMove(float x, float y) const;

	sf::Vector2f _position;

	Animation _walkAnimation;

	float _angle;
	float _moveSpeed;
	float _rotationSpeed;

	float _radius;
};

#endif // !PLAYER_H
