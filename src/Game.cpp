#include <cmath>
#include <iostream>
#include <algorithm>

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

    _playerLoaded = _playerTexture.loadFromFile("assets/newPlayer.png");

    if (!_playerLoaded)
    {
        std::cout << "Failed to load player.png\n";
    }

    _cameraDistance = 150.0f;
    _cameraPosition = _player.getPosition();
    _cameraAngle = _player.getAngle();
    _cameraPitch = 0.0f;
    _cameraSensitivity = 0.003f;
    _pitchSensitivity = 0.15f;

    _mouseCenter = sf::Vector2i(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

    sf::Mouse::setPosition(_mouseCenter, _window);
    _window.setMouseCursorVisible(false);
}

sf::Vector2f Game::getSafeCameraPosition(
    const sf::Vector2f& playerPosition,
    const sf::Vector2f& desiredPosition)
{
    sf::Vector2f direction =
        desiredPosition - playerPosition;

    float distance =
        std::sqrt(
            direction.x * direction.x +
            direction.y * direction.y
        );

    if (distance <= 0.0f)
        return playerPosition;

    direction /= distance;

    float safeDistance = distance;

    const float step = 5.0f;

    for (float d = 0.0f; d < distance; d += step)
    {
        float x =
            playerPosition.x + direction.x * d;

        float y =
            playerPosition.y + direction.y * d;

        int mapX =
            static_cast<int>(x / Map::TILE_SIZE);

        int mapY =
            static_cast<int>(y / Map::TILE_SIZE);

        if (Map::isWall(mapX, mapY))
        {
            safeDistance = d - 10.0f;
            break;
        }
    }

    if (safeDistance < 20.0f)
        safeDistance = 20.0f;

    return playerPosition + direction * safeDistance;
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
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                    _window.close();
            }
        }

        update(deltaTime);
        render();
    }
}

void Game::update(float deltaTime)
{
    //Mouse camera
    sf::Vector2i mousePosition =
        sf::Mouse::getPosition(_window);

    int mouseDeltaX = mousePosition.x - _mouseCenter.x;
    int mouseDeltaY = mousePosition.y - _mouseCenter.y;
    
    _cameraAngle += mouseDeltaX * _cameraSensitivity;
    _cameraPitch -= mouseDeltaY * _pitchSensitivity;

    if (_cameraPitch > 300.0f)
        _cameraPitch = 300.0f;

    if (_cameraPitch < -300.0f)
        _cameraPitch = -300.0f;

    sf::Mouse::setPosition(_mouseCenter, _window);

    // Player movement
    _player.update(deltaTime, _cameraAngle);
    _world.update(deltaTime);

    // CAMERA POSITION
    sf::Vector2f playerPos =
        _player.getPosition();

    // Camera follows the direction
    // that the CAMERA is looking
    sf::Vector2f cameraForward(
        std::cos(_cameraAngle),
        std::sin(_cameraAngle)
    );

    //Where the camera wants to be
    sf::Vector2f desiredCameraPosition =
        playerPos - cameraForward * _cameraDistance;

    desiredCameraPosition = getSafeCameraPosition(playerPos, desiredCameraPosition);

    //Smooth camera movement
    float followSpeed = 8.0f;

    float smooth = std::min(1.0f, followSpeed * deltaTime);
    _cameraPosition += (desiredCameraPosition - _cameraPosition) * smooth;
}

void Game::renderTrees()
{
    if (!_treeLoaded)
        return;

    //sf::Vector2f playerPos = _player.getPosition();
    sf::Vector2f cameraPos = _cameraPosition;

    //float playerAngle = _player.getAngle();
    float cameraAngle = _cameraAngle;

    const std::vector<float>& depthBuffer = _raycaster.getDepthBuffer();

    const float horizon =
        SCREEN_HEIGHT / 2.0f + _cameraPitch;

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
            treePos.x - cameraPos.x;

        float dy =
            treePos.y - cameraPos.y;


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
            treeAngle - cameraAngle;


        // Normalize angle
        while (angleDifference > PI)
            angleDifference -= 2.0f * PI;

        while (angleDifference < -PI)
            angleDifference += 2.0f * PI;

        float correctedTreeDistance = distance * std::cos(angleDifference);

        if (correctedTreeDistance <= 0.1f)
            continue;

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

    //sf::Vector2f playerPosition = _player.getPosition();
    sf::Vector2f cameraPosition = _cameraPosition;

    //float playerAngle = _player.getAngle();
    float cameraAngle = _cameraAngle;

    for (const Rock& rock : rocks)
    {
        sf::Vector2f rockPosition = rock.getPosition();

        float dx = rockPosition.x - cameraPosition.x;

        float dy = rockPosition.y - cameraPosition.y;

        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance < 1.0f)
            continue;

        float rockAngle = std::atan2(dy, dx);

        float angleDifference = rockAngle - cameraAngle;

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

        const float horizon =
            SCREEN_HEIGHT / 2.0f + _cameraPitch;

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

void Game::renderPlayer()
{
    if (!_playerLoaded)
        return;

    const std::vector<float>& depthBuffer = _raycaster.getDepthBuffer();

    sf::Vector2f playerPosition = _player.getPosition();

    // VECTOR FROM CAMERA TO PLAYER

    float dx = playerPosition.x - _cameraPosition.x;
    float dy = playerPosition.y - _cameraPosition.y;

    float distance = std::sqrt(dx * dx + dy * dy);
    if (distance < 1.0f)
        return;

    // ANGLE TO PLAYER

    float playerWorldAngle =
        std::atan2(dy, dx);

    float angleDifference =
        playerWorldAngle - _cameraAngle;

    // Normalize angle
    while (angleDifference > PI)
        angleDifference -= 2.0f * PI;

    while (angleDifference < -PI)
        angleDifference += 2.0f * PI;

    // OUTSIDE CAMERA
    if (std::abs(angleDifference) > FOV / 2.0f)
        return;

    // FISHEYE CORRECTION
    float correctedDistance =
        distance * std::cos(angleDifference);

    if (correctedDistance <= 0.1f)
        return;

    // SCREEN X
    float screenX =
        ((angleDifference + FOV / 2.0f) / FOV)
        * SCREEN_WIDTH;

    // PLAYER SIZE
    const float playerWorldHeight = 64.0f;

    float playerHeight =
        (playerWorldHeight * 500.0f)
        / correctedDistance;

    float imageWidth =
        static_cast<float>(_playerImage.getSize().x);

    float imageHeight =
        static_cast<float>(_playerImage.getSize().y);

    float aspectRatio =
        imageWidth / imageHeight;

    float playerWidth =
        playerHeight * aspectRatio;

    // Don't let the player become enormous
    if (playerHeight > SCREEN_HEIGHT * 2)
    {
        playerHeight = SCREEN_HEIGHT * 2;

        playerWidth =
            playerHeight * aspectRatio;
    }

    // GROUND POSITION
    const float horizon =
        SCREEN_HEIGHT / 2.0f + _cameraPitch;

    float playerBottom =
        horizon +
        (CAMERA_HEIGHT * 500.0f)
        / correctedDistance;

    float playerTop =
        playerBottom - playerHeight;

    float playerLeft =
        screenX - playerWidth / 2.0f;

    // DRAW SPRITE
    for (int y = 0;
        y < static_cast<int>(playerHeight);
        y++)
    {
        int screenY =
            static_cast<int>(playerTop) + y;

        if (screenY < 0 ||
            screenY >= SCREEN_HEIGHT)
        {
            continue;
        }

        unsigned int textureY =
            static_cast<unsigned int>(
                (static_cast<float>(y) /
                    playerHeight)
                * imageHeight
                );

        if (textureY >= imageHeight)
            textureY = imageHeight - 1;

        for (int x = 0;
            x < static_cast<int>(playerWidth);
            x++)
        {
            int screenXPixel =
                static_cast<int>(playerLeft) + x;

            if (screenXPixel < 0 ||
                screenXPixel >= SCREEN_WIDTH)
            {
                continue;
            }

            unsigned int textureX =
                static_cast<unsigned int>(
                    (static_cast<float>(x) /
                        playerWidth)
                    * imageWidth
                    );

            if (textureX >= imageWidth)
                textureX = imageWidth - 1;

            sf::Color pixel =
                _playerImage.getPixel(
                    textureX,
                    textureY
                );

            // Transparent pixels don't get drawn
            if (pixel.a <= 10)
                continue;

            // =========================
            // DEPTH TEST
            // =========================

            if (correctedDistance >
                depthBuffer[screenXPixel] - 1.0f)
            {
                continue;
            }

            _renderer.setPixel(
                screenXPixel,
                screenY,
                pixel
            );
        }
    }
}

void Game::render()
{
    _renderer.clear(sf::Color::Black);

    const int horizon = SCREEN_HEIGHT / 2;
    float projectedHorizon = SCREEN_HEIGHT / 2.0f + _cameraPitch;

    // Draw sky
    for (int y = 0; y < SCREEN_HEIGHT; y++)
    {
        if (y >= projectedHorizon)
            break;

        int blue = 180 + (y * 50 / SCREEN_HEIGHT);

        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            _renderer.setPixel(x, y, sf::Color(100, 180, blue));
        }
    }

    // Player Camera
    //sf::Vector2f playerPos = _player.getPosition();
    sf::Vector2f cameraPos = _cameraPosition;

    //float playerAngle = _player.getAngle();
    float cameraAngle = _cameraAngle;

    float leftAngle = cameraAngle - FOV / 2.0f;

    float rightAngle = cameraAngle + FOV / 2.0f;

    // Draw Grass Floor
    int floorStart =
        static_cast<int>(projectedHorizon) + 1;

    if (floorStart < 0)
        floorStart = 0;

    if (floorStart >= SCREEN_HEIGHT)
        floorStart = SCREEN_HEIGHT - 1;

    for (int y = floorStart; y < SCREEN_HEIGHT; y++)
    {
        // Distance from the camera
        //float distance = CAMERA_HEIGHT / (static_cast<float>(y) - projectedHorizon);
        float distance =
            CAMERA_HEIGHT /
            (static_cast<float>(y) - projectedHorizon);

        // Left side of camera
        //float leftX = playerPos.x + std::cos(leftAngle) * distance;
        //float leftY = playerPos.y + std::sin(leftAngle) * distance;

        //Right side of camera
        //float rightX = playerPos.x + std::cos(rightAngle) * distance;
        //float rightY = playerPos.y + std::sin(rightAngle) * distance;

        float leftX = cameraPos.x + std::cos(leftAngle) * distance;
        float leftY = cameraPos.y + std::sin(leftAngle) * distance;

        float rightX = cameraPos.x + std::cos(rightAngle) * distance;
        float rightY = cameraPos.y + std::sin(rightAngle) * distance;

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

    _raycaster.castRays(_cameraPosition, _cameraAngle, _cameraPitch);

    const std::vector<float>& depthBuffer =
        _raycaster.getDepthBuffer();

    for (const House& house : _world.getHouses())
    {
        _houseRenderer.render(
            house,
            _cameraPosition,
            _cameraAngle,
            _cameraPitch,
            depthBuffer
        );
    }

    renderRocks();
    renderTrees();
    renderPlayer();

    _renderer.display(_window);

    _window.display();
}