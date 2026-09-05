#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>

#include "Player.h"
#include "Renderer.h"
#include "Map.h"
#include "Raycaster.h"
#include "World.h"

class Game
{
public:
    Game();

    void run();

private:
    void update(float deltaTime);
    void render();

    void renderTrees();

    sf::RenderWindow _window;

    Player    _player;

    Renderer  _renderer;

    Raycaster _raycaster;

    World     _world;

    sf::Image _treeImage;

    bool _treeLoaded;
};

#endif