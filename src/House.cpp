#include "include/House.h"

House::House(float x, float y, float width, float height)
    : _position(x, y),
    _width(width),
    _height(height)
{
    // =========================
    // WINDOWS
    // =========================

    _windowWidth = 40.0f;
    _windowHeight = 40.0f;

    // North wall
    // Left and right of the door

    _window1Position = sf::Vector2f(
        x - 80.0f,
        y - height / 2.0f
    );

    _window2Position = sf::Vector2f(
        x + 80.0f,
        y - height / 2.0f
    );


    // =========================
    // DOOR
    // =========================

    _doorWidth = 40.0f;
    _doorHeight = 50.0f;

    _doorPosition = sf::Vector2f(
        x,
        y - height / 2.0f
    );
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

sf::Vector2f House::getWindow1Position() const
{
    return _window1Position;
}


sf::Vector2f House::getWindow2Position() const
{
    return _window2Position;
}

float House::getWindowWidth() const
{
    return _windowWidth;
}


float House::getWindowHeight() const
{
    return _windowHeight;
}

sf::Vector2f House::getDoorPosition() const
{
    return _doorPosition;
}

float House::getDoorWidth() const
{
    return _doorWidth;
}

float House::getDoorHeight() const
{
    return _doorHeight;
}