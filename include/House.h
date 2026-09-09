#ifndef HOUSE_H
#define HOUSE_H

#include <SFML/System.hpp>

class House
{
public:
    House(
        float x,
        float y,
        float width,
        float height
    );

    sf::Vector2f getPosition() const;

    float getWidth() const;
    float getHeight() const;

    sf::Vector2f getWindow1Position() const;
    sf::Vector2f getWindow2Position() const;

    float getWindowWidth() const;
    float getWindowHeight() const;

    // Door
    sf::Vector2f getDoorPosition() const;
    float getDoorWidth() const;
    float getDoorHeight() const;

private:
    sf::Vector2f _position;

    sf::Vector2f _window1Position;
    sf::Vector2f _window2Position;

    float _windowWidth;
    float _windowHeight;

    float _width;
    float _height;

    sf::Vector2f _doorPosition;

    float _doorWidth;
    float _doorHeight;
};

#endif // !HOUSE_H