#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>

#include "Player.h"
#include "Renderer.h"
#include "Map.h"
#include "Raycaster.h"
#include "World.h"
#include "HouseRenderer.h"

class Game
{
public:
    Game();

    void run();

private:
    void update(float deltaTime);
    void render();

    void renderTrees();
    void renderRocks();

    sf::RenderWindow _window;

    Player    _player;

    Renderer  _renderer;

    Raycaster _raycaster;

    World     _world;

    HouseRenderer _houseRenderer;

    sf::Image _treeImage;

    bool _treeLoaded;
};

#endif