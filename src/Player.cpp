#include "include/Player.h"
#include "include/Map.h"

#include <SFML/Window/Keyboard.hpp>
#include <cmath>

Player::Player(float x, float y)
{
	_position = sf::Vector2f(x, y);

	_angle = 0.0f;

	_moveSpeed = 200.0f;
	_rotationSpeed = 2.0f;
	_radius = 15.0f;
    _walkAnimation.setFrameCount(3);
    _walkAnimation.setFrameDuration(0.12f);
}

bool Player::canMove(float x, float y) const
{
	// Check four corners around the player

	const float points[4][2] =
	{
		{ x - _radius, y - _radius },
		{ x + _radius, y - _radius },
		{ x - _radius, y + _radius },
		{ x + _radius, y + _radius }
	};


	for (const auto& point : points)
	{
		int mapX =
			static_cast<int>(
				point[0] / Map::TILE_SIZE
				);

		int mapY =
			static_cast<int>(
				point[1] / Map::TILE_SIZE
				);


		if (Map::isWall(mapX, mapY))
		{
			return false;
		}
	}

	return true;
}

void Player::update(float deltaTime, float cameraAngle)
{
    // =========================
    // ROTATION
    // =========================

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        _angle -= _rotationSpeed * deltaTime;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        _angle += _rotationSpeed * deltaTime;
    }


    // =========================
    // CAMERA DIRECTION
    // =========================

    float dirX = std::cos(cameraAngle);
    float dirY = std::sin(cameraAngle);


    // =========================
    // MOVEMENT VECTOR
    // =========================

    float moveX = 0.0f;
    float moveY = 0.0f;


    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        moveX += dirX;
        moveY += dirY;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        moveX -= dirX;
        moveY -= dirY;
    }


    // Strafe left

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        moveX += dirY;
        moveY -= dirX;
    }


    // Strafe right

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        moveX -= dirY;
        moveY += dirX;
    }


    // =========================
    // NORMALIZE MOVEMENT
    // =========================

    float length =
        std::sqrt(moveX * moveX +
            moveY * moveY);

    if (length > 0.0f)
    {       
        moveX /= length;
        moveY /= length;

        // Player turns toward movement direction
        _angle = std::atan2(moveY, moveX);

        _walkAnimation.update(deltaTime);

        moveX *= _moveSpeed * deltaTime;
        moveY *= _moveSpeed * deltaTime;

        float newX = _position.x + moveX;
        float newY = _position.y + moveY;

        // X collision
        if (canMove(newX, _position.y))
            _position.x = newX;

        // Y collision
        if (canMove(_position.x, newY))
            _position.y = newY;
    }
    else
    {
        // Player isn't moving
        _walkAnimation.reset();
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