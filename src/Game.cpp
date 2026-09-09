#include <cmath>
#include <iostream>

#include "../include/Game.h"
#include "../include/Config.h"

Game::Game()
    : _window(
        sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT),
        "VoxelRay"
    ),
    _player(200.0f, 200.0f),
    _renderer(SCREEN_WIDTH, SCREEN_HEIGHT),
    _raycaster(_renderer),
    _world(),
    _houseRenderer(_renderer),
    _treeLoaded(false)
{
    _window.setFramerateLimit(144);

    _treeLoaded = _treeImage.loadFromFile("assets/tree.png");

    if (!_treeLoaded)
    {
        std::cout << "Failed to load tree.png\n";
    }
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
    _world.update(deltaTime);
}

void Game::renderTrees()
{
    if (!_treeLoaded)
        return;

    sf::Vector2f playerPos = _player.getPosition();

    float playerAngle = _player.getAngle();

    const std::vector<float>& depthBuffer = _raycaster.getDepthBuffer();

    const int horizon = SCREEN_HEIGHT / 2;

    // Original image dimensions
    unsigned int imageWidth =
        _treeImage.getSize().x;

    unsigned int imageHeight =
        _treeImage.getSize().y;


    for (const Tree& tree : _world.getTrees())
    {
        sf::Vector2f treePos =
            tree.getPosition();


        // =========================
        // VECTOR TO TREE
        // =========================

        float dx =
            treePos.x - playerPos.x;

        float dy =
            treePos.y - playerPos.y;


        float distance =
            std::sqrt(dx * dx + dy * dy);


        // Don't divide by zero
        if (distance < 1.0f)
            distance = 1.0f;


        // =========================
        // TREE ANGLE
        // =========================

        float treeAngle =
            std::atan2(dy, dx);

        float angleDifference =
            treeAngle - playerAngle;


        // Normalize angle
        while (angleDifference > PI)
            angleDifference -= 2.0f * PI;

        while (angleDifference < -PI)
            angleDifference += 2.0f * PI;

        float correctedTreeDistance = distance * std::cos(angleDifference);


        // Outside camera view
        if (std::abs(angleDifference) >
            FOV / 2.0f)
        {
            continue;
        }


        // =========================
        // SCREEN POSITION
        // =========================

        float screenX =
           (angleDifference + FOV / 2.0f)
           / FOV
          * SCREEN_WIDTH;


        // =========================
        // PERSPECTIVE SCALE
        // =========================

        float treeHeight =
           25000.0f / correctedTreeDistance;

        float aspectRatio =
            static_cast<float>(imageWidth) /
            static_cast<float>(imageHeight);

        float treeWidth =
            treeHeight * aspectRatio;


        // Limit maximum size
        if (treeHeight > SCREEN_HEIGHT * 2)
        {
            treeHeight = SCREEN_HEIGHT * 2;

            treeWidth =
                treeHeight * aspectRatio;
        }


        // =========================
        // TREE POSITION
        // =========================

        // Bottom of tree sits on ground
        float treeBottom =
            horizon + treeHeight / 2.0f;

        float treeTop =
            treeBottom - treeHeight;

        float treeLeft =
            screenX - treeWidth / 2.0f;


        // =========================
        // DRAW SPRITE
        // =========================

        for (int y = 0;
            y < static_cast<int>(treeHeight);
            y++)
        {
            int screenY =
                static_cast<int>(treeTop) + y;

            if (screenY < 0 ||
                screenY >= SCREEN_HEIGHT)
            {
                continue;
            }


            // Convert screen Y → image Y
            unsigned int textureY =
                static_cast<unsigned int>(
                    (static_cast<float>(y) /
                        treeHeight) *
                    imageHeight
                    );

            if (textureY >= imageHeight)
                textureY = imageHeight - 1;


            for (int x = 0;
                x < static_cast<int>(treeWidth);
                x++)
            {
                int screenXPixel =
                    static_cast<int>(treeLeft) + x;

                if (screenXPixel < 0 ||
                    screenXPixel >= SCREEN_WIDTH)
                {
                    continue;
                }


                // Convert screen X → image X
                unsigned int textureX =
                    static_cast<unsigned int>(
                        (static_cast<float>(x) /
                            treeWidth) *
                        imageWidth
                        );

                if (textureX >= imageWidth)
                    textureX = imageWidth - 1;


                sf::Color pixel =
                    _treeImage.getPixel(
                        textureX,
                        textureY
                    );


                // Draw only visible pixels
                if (pixel.a > 10)
                {
                    // Check if the tree is closer
                    // than the wall at this screen column

                    const float DEPTH_BIAS = 1.0f;

                    if (correctedTreeDistance < depthBuffer[screenXPixel] - DEPTH_BIAS)
                    {
                        _renderer.setPixel(
                            screenXPixel,
                            screenY,
                            pixel
                        );
                    }
                }
            }
        }
    }
}

void Game::renderRocks()
{
    const auto& rocks = _world.getRocks();
    const auto& depthBuffer = _raycaster.getDepthBuffer();

    sf::Vector2f playerPosition = _player.getPosition();

    float playerAngle = _player.getAngle();

    for (const Rock& rock : rocks)
    {
        sf::Vector2f rockPosition = rock.getPosition();

        float dx = rockPosition.x - playerPosition.x;

        float dy = rockPosition.y - playerPosition.y;

        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance < 1.0f)
            continue;

        float rockAngle = std::atan2(dy, dx);

        float angleDifference = rockAngle - playerAngle;

        //keep angle between -PI and +PI
        while (angleDifference > PI)
            angleDifference -= 2.0f * PI;

        while (angleDifference < -PI)
            angleDifference += 2.0f * PI;

        // Outside the player's view
        if (std::abs(angleDifference) > FOV / 2.0f)
            continue;

        //Correct distance for fisheye
        float correctedDistance = distance * std::cos(angleDifference);

        if (correctedDistance <= 0.1f)
            continue;

        //Horizontal screen position;
        float screenX = ((angleDifference + FOV / 2.0f) / FOV) * SCREEN_WIDTH;

        int centerX = static_cast<int>(screenX);

        if (centerX < 0 ||
            centerX >= SCREEN_WIDTH)
            continue;

        const int horizon = SCREEN_HEIGHT / 2;

        // Physical size of the rock in world units
        float rockWorldHeight = 24.0f;

        //Rock size
        float rockHeight =
            (rockWorldHeight * 500.0f) /
            correctedDistance;
        float rockWidth = rockHeight * 1.3f;
        // Where the ground is at this distance
        float rockBottom =
            horizon +
            (CAMERA_HEIGHT * 500.0f) /
            correctedDistance + 5.0f;
        float rockTop = rockBottom - rockHeight;
        int left = static_cast<int>(screenX - rockWidth / 2.0f);
        int right = static_cast<int>(screenX + rockWidth / 2.0f);

        int topPixel = static_cast<int>(rockTop);
        int bottomPixel = static_cast<int>(rockBottom);

        for (int y = topPixel; y <= bottomPixel; y++)
        {
            if (y < 0 || y >= SCREEN_HEIGHT)
                continue;

            for (int x = left; x <= right; x++)
            {
                if (x < 0 || x >= SCREEN_WIDTH)
                    continue;

                //Convert pixel to -1..1
                float nx =
                    (x - screenX) /
                    (rockWidth / 2.0f);

                float ny =
                    (y - (rockTop + rockHeight / 2.0f)) /
                    (rockHeight / 2.0f);

                // Ellipse shape
                if (nx * nx + ny * ny > 1.0f)
                    continue;

                // Depth test against walls
                if (correctedDistance >
                    depthBuffer[x] + 2.0f)
                {
                    continue;
                }

                // Simple pixel-art shading
                sf::Color rockColor;

                if (ny < -0.3f)
                {
                    rockColor =
                        sf::Color(130, 130, 130);
                }
                else if (ny < 0.3f)
                {
                    rockColor =
                        sf::Color(100, 100, 100);
                }
                else
                {
                    rockColor =
                        sf::Color(65, 65, 65);
                }

                _renderer.setPixel(
                    x,
                    y,
                    rockColor
                );
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

    const std::vector<float>& depthBuffer =
        _raycaster.getDepthBuffer();

    for (const House& house : _world.getHouses())
    {
        _houseRenderer.render(
            house,
            _player,
            depthBuffer
        );
    }

    renderRocks();
    renderTrees();

    _renderer.display(_window);

    _window.display();
}