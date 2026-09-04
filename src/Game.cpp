#include <cmath>

#include "../include/Game.h"
#include "../include/Config.h"

Game::Game()
    : _window(
        sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT),
        "VoxelRay"
    ),
    _player(400.0f, 300.0f),
    _renderer(SCREEN_WIDTH, SCREEN_HEIGHT),
    _raycaster(_renderer)
{
    _window.setFramerateLimit(144);

    Map::init();

    _trees.emplace_back(600.0f, 300.0f);
    _trees.emplace_back(800.0f, 400.0f);
    _trees.emplace_back(500.0f, 600.0f);
    _trees.emplace_back(900.0f, 200.0f);
    _trees.emplace_back(200.0f, 500.0f);
}

void Game::run()
{
    sf::Clock clock;

    while (_window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        sf::Event event;

        while (_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                _window.close();
            }
        }

        update(deltaTime);
        render();
    }
}

void Game::update(float deltaTime)
{
    _player.update(deltaTime);
}

void Game::renderTrees()
{
    sf::Vector2f playerPos = _player.getPosition();

    float playerAngle = _player.getAngle();

    const int horizon = SCREEN_HEIGHT / 2;

    for (const Tree& tree : _trees)
    {
        sf::Vector2f treePos = tree.getPosition();

        //Vector from player to tree
        float dx = treePos.x - playerPos.x;
        float dy = treePos.y - playerPos.y;

        //Distance to tree
        float distance = std::sqrt(dx * dx + dy * dy);

        //Angle towards the tree
        float treeAngle = std::atan2(dy, dx);

        //Angle difference from camera
        float angleDifference = treeAngle - playerAngle;

        //Normalize angle
        while (angleDifference > PI)
        {
            angleDifference -= 2.0f * PI;
        }

        while (angleDifference < -PI)
        {
            angleDifference += 2.0f * PI;
        }

        //Dont render trees outside the FOV
        if (std::abs(angleDifference) > FOV / 2.0f)
        {
            continue;
        }

        //Project Tree on Screen

        //Convert angle to screen X position
        float screenX = (angleDifference + FOV / 2.0f) / FOV * SCREEN_WIDTH;

        //Perspective size
        float treeHeight = 8000.0f / distance;

        float treeWidth = treeHeight * 0.6f;

        //Prevent extremely large trees
        if (treeHeight > SCREEN_HEIGHT * 2)
        {
            treeHeight = SCREEN_HEIGHT * 2;
        }

        //Tree bottom sits on horizon/ground
        float treeBottom = horizon + treeHeight / 2.0f;
        float treeTop = treeBottom - treeHeight;

        float treeLeft = screenX - treeWidth / 2.0f;

        //Draw tree
        for (int y = static_cast<int>(treeTop); y < static_cast<int>(treeBottom); y++)
        {
            if (y < 0 || y >= SCREEN_HEIGHT)
                continue;

            for (int x = static_cast<int>(treeLeft); x < static_cast<int>(treeLeft + treeWidth); x++)
            {
                if (x < 0 || x >= SCREEN_WIDTH)
                    continue;

                float normalizedY = (y - treeTop) / treeHeight;

                //Top = leaves
                if (normalizedY < 0.70f)
                {
                    _renderer.setPixel(x, y, sf::Color(30, 120, 40));
                }
                else
                {
                    //Bottom = trunk
                    float center = screenX;
                    float trunkWidth = treeWidth * 0.25f;

                    if (x > center - trunkWidth / 2 &&
                        x < center + trunkWidth / 2)
                    {
                        _renderer.setPixel(x, y, sf::Color(100, 60, 20));
                    }
                }
            }
        }
    }
}

void Game::render()
{
    _renderer.clear(sf::Color::Black);

    const int horizon = SCREEN_HEIGHT / 2;

    // Draw sky
    for (int y = 0; y < horizon; y++)
    {
        int blue = 180 + (y * 50 / horizon);

        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            _renderer.setPixel(x, y, sf::Color(100, 180, blue));
        }
    }

    // Player Camera
    sf::Vector2f playerPos = _player.getPosition();

    float playerAngle = _player.getAngle();

    float leftAngle = playerAngle - FOV / 2.0f;

    float rightAngle = playerAngle + FOV / 2.0f;

    // Draw Grass Floor

    for (int y = horizon + 1; y < SCREEN_HEIGHT; y++)
    {
        // Distance from the camera
        float distance = CAMERA_HEIGHT / static_cast<float>(y - horizon);

        // Left side of camera
        float leftX = playerPos.x + std::cos(leftAngle) * distance;
        float leftY = playerPos.y + std::sin(leftAngle) * distance;

        //Right side of camera
        float rightX = playerPos.x + std::cos(rightAngle) * distance;
        float rightY = playerPos.y + std::sin(rightAngle) * distance;

        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            float t = static_cast<float>(x) / static_cast<float>(SCREEN_WIDTH - 1);

            //Interpolate between left and right rays
            float worldX = leftX + (rightX - leftX) * t;
            float worldY = leftY + (rightY - leftY) * t;

            //PROCEDURAL GRASS PATTERN

            int grassPattern = 
                (static_cast<int>(worldX / 16.0f) +
                 static_cast<int>(worldY / 16.0f)) % 2;

            if (grassPattern == 0)
            {
                _renderer.setPixel(x, y, sf::Color(45, 130, 45));
            }
            else
            {
                _renderer.setPixel(x, y, sf::Color(55, 155, 55));
            }
        }
    }

    _raycaster.castRays(_player.getPosition(), _player.getAngle());

    renderTrees();

    _renderer.display(_window);

    _window.display();
}