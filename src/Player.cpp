#include "include/Player.h"

#include <SFML/Window/Keyboard.hpp>
#include <cmath>

Player::Player(float x, float y)
{
	_position = sf::Vector2f(x, y);

	_angle = 0.0f;

	_moveSpeed = 200.0f;
	_rotationSpeed = 2.0f;
}

void Player::update(float deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		_angle -= _rotationSpeed * deltaTime;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		_angle += _rotationSpeed * deltaTime;
	}

	float dirX = std::cos(_angle);
	float dirY = std::sin(_angle);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		_position.x += dirX * _moveSpeed * deltaTime;
		_position.y += dirY * _moveSpeed * deltaTime;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		_position.x -= dirX * _moveSpeed * deltaTime;
		_position.y -= dirY * _moveSpeed * deltaTime;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		_position.x += dirY * _moveSpeed * deltaTime;
		_position.y -= dirX * _moveSpeed * deltaTime;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		_position.x -= dirY * _moveSpeed * deltaTime;
		_position.y += dirX * _moveSpeed * deltaTime;
	}
}

sf::Vector2f Player::getPosition() const
{
	return _position;
}

float Player::getAngle() const
{
	return _angle;
}