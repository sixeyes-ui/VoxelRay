#ifndef HOUSE_RENDERER_H
#define HOUSE_RENDERER_H

#include <SFML/System.hpp>
#include <vector>

class Renderer;
class House;
class Player;

class HouseRenderer
{
public:
    HouseRenderer(Renderer& renderer);

    void render(
        const House& house,
        const sf::Vector2f& cameraPosition,
        float cameraAngle,
        const std::vector<float>& depthBuffer
    );

private:
    Renderer& _renderer;
};

#endif // !HOUSE_RENDERER_H