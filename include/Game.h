#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>

#include "Player.h"
#include "Renderer.h"
#include "Tree.h"
#include "Map.h"
#include "Raycaster.h"

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

    Player _player;

    Renderer _renderer;

    Raycaster _raycaster;

    std::vector<Tree> _trees;
};

#endif