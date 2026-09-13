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
    void renderPlayer();

    sf::Vector2f getSafeCameraPosition(
        const sf::Vector2f& playerPosition,
        const sf::Vector2f& desiredPosition
    );

    sf::RenderWindow _window;

    Player    _player;

    Renderer  _renderer;

    Raycaster _raycaster;

    World     _world;

    HouseRenderer _houseRenderer;

    sf::Image _treeImage;

    //sf::Image _playerImage;
    sf::Texture _playerTexture;
    bool _playerLoaded;

    sf::Vector2f _cameraPosition;
    float        _cameraDistance;
    float        _cameraAngle;
    float        _cameraPitch;

    float        _cameraSensitivity;
    float        _pitchSensitivity;

    sf::Vector2i _mouseCenter;

    float        _angle;

    bool _treeLoaded;
};

#endif