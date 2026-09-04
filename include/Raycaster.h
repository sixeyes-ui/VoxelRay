#ifndef RAYCASTER_H
#define RAYCASTER_H

#include <SFML/System.hpp>

class Renderer;

class Raycaster
{
public:
    Raycaster(Renderer& renderer);

    void castRays(
        const sf::Vector2f& position,
        float angle
    );

private:
    Renderer& _renderer;
};

#endif