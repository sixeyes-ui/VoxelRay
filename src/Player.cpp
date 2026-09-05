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

void Player::update(float deltaTime)
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

    float dirX = std::cos(_angle);
    float dirY = std::sin(_angle);


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


        moveX *= _moveSpeed * deltaTime;
        moveY *= _moveSpeed * deltaTime;


        float newX =
            _position.x + moveX;

        float newY =
            _position.y + moveY;


        // =========================
        // COLLISION
        // =========================

        // Move X separately
        // This allows wall sliding

        if (canMove(newX, _position.y))
        {
            _position.x = newX;
        }


        // Move Y separately

        if (canMove(_position.x, newY))
        {
            _position.y = newY;
        }
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