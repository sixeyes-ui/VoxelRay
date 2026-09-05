#ifndef RAYCASTER_H
#define RAYCASTER_H

#include <SFML/System.hpp>
#include <vector>

class Renderer;

class Raycaster
{
public:
    Raycaster(Renderer& renderer);

    void castRays(
        const sf::Vector2f& position,
        float angle
    );

    const std::vector<float>& getDepthBuffer() const;

private:
    Renderer& _renderer;

    std::vector<float> _depthBuffer;
};

#endif