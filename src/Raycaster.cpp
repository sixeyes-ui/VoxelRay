#include "../include/Raycaster.h"

#include "../include/Config.h"
#include "../include/Map.h"
#include "../include/Renderer.h"

#include <iostream>
#include <cmath>

Raycaster::Raycaster(Renderer& renderer)
    : _renderer(renderer),
      _houseTextureLoaded(false)
{
    _depthBuffer.resize(SCREEN_WIDTH);

    _houseTextureLoaded = _houseTexture.loadFromFile("assets/wall.png");

    if (!_houseTextureLoaded)
    {
        std::cout << "Failed to load wall.png\n";
    }
}

const std::vector<float>& Raycaster::getDepthBuffer() const
{
    return _depthBuffer;
}

void Raycaster::castRays(
    const sf::Vector2f& position,
    float playerAngle)
{
    const int NUM_RAYS = SCREEN_WIDTH;

    const float angleStep =
        FOV / static_cast<float>(NUM_RAYS);

    float rayAngle =
        playerAngle - FOV / 2.0f;


    for (int ray = 0;
        ray < NUM_RAYS;
        ray++)
    {
        float rayDirX = std::cos(rayAngle);
        float rayDirY = std::sin(rayAngle);


        // =========================
        // PLAYER MAP POSITION
        // =========================

        int mapX =
            static_cast<int>(position.x / Map::TILE_SIZE);

        int mapY =
            static_cast<int>(position.y / Map::TILE_SIZE);


        // =========================
        // DDA DISTANCE
        // =========================

        float deltaDistX =
            std::abs(Map::TILE_SIZE / rayDirX);

        float deltaDistY =
            std::abs(Map::TILE_SIZE / rayDirY);


        int stepX;
        int stepY;

        float sideDistX;
        float sideDistY;


        // X direction

        if (rayDirX < 0)
        {
            stepX = -1;

            sideDistX =
                (position.x -
                    mapX * Map::TILE_SIZE)
                / -rayDirX;
        }
        else
        {
            stepX = 1;

            sideDistX =
                ((mapX + 1) * Map::TILE_SIZE -
                    position.x)
                / rayDirX;
        }


        // Y direction

        if (rayDirY < 0)
        {
            stepY = -1;

            sideDistY =
                (position.y -
                    mapY * Map::TILE_SIZE)
                / -rayDirY;
        }
        else
        {
            stepY = 1;

            sideDistY =
                ((mapY + 1) * Map::TILE_SIZE -
                    position.y)
                / rayDirY;
        }


        // =========================
        // DDA LOOP
        // =========================

        bool hit = false;
        int side = 0;

        while (!hit)
        {
            if (sideDistX < sideDistY)
            {
                sideDistX += deltaDistX;

                mapX += stepX;

                side = 0;
            }
            else
            {
                sideDistY += deltaDistY;

                mapY += stepY;

                side = 1;
            }

            if (Map::isWall(mapX, mapY))
            {
                hit = true;
            }
        }


        // =========================
        // DISTANCE TO WALL
        // =========================

        float rawDistance;

        if (side == 0)
        {
            rawDistance =
                sideDistX - deltaDistX;
        }
        else
        {
            rawDistance =
                sideDistY - deltaDistY;
        }

        // Distance used for projection/depth
        float angleDifference =
            rayAngle - playerAngle;

        float distance =
            rawDistance * std::cos(angleDifference);

        if (distance < 0.1f)
            distance = 0.1f;

        _depthBuffer[ray] = distance;

        // =========================
        // PROJECT WALL
        // =========================

        float wallHeight =
            (Map::TILE_SIZE * 500.0f)
            / distance;


        int wallTop =
            static_cast<int>(
                SCREEN_HEIGHT / 2.0f -
                wallHeight / 2.0f
                );

        int wallBottom =
            static_cast<int>(
                SCREEN_HEIGHT / 2.0f +
                wallHeight / 2.0f
                );

        float wallX;

        if (side == 0)
        {
            wallX = position.y + rawDistance * rayDirY;
        }
        else
        {
            wallX = position.x + rawDistance * rayDirX;
        }

        wallX /= Map::TILE_SIZE;

        wallX -= std::floor(wallX);

        // =========================
        // DRAW WALL COLUMN
        // =========================

        bool isHouse = Map::isHouse(mapX, mapY);

        sf::Color wallColor;

        if (!isHouse)
        {
            if (side == 0)
                wallColor = sf::Color(100, 100, 100);
            else
                wallColor = sf::Color(70, 70, 70);
        }


        for (int y = wallTop;
            y < wallBottom;
            y++)
        {
            if (y < 0 || y >= SCREEN_HEIGHT)
                continue;

            sf::Color pixelColor;

            if (isHouse && _houseTextureLoaded)
            {
                // Horizontal texture coordinate
                int textureX =
                    static_cast<int>(
                        wallX * _houseTexture.getSize().x
                        );

                // Vertical texture coordinate
                float wallPosition =
                    static_cast<float>(y - wallTop) /
                    wallHeight;

                int textureY =
                    static_cast<int>(
                        wallPosition *
                        _houseTexture.getSize().y
                        );

                // Safety
                if (textureX < 0)
                    textureX = 0;

                if (textureX >=
                    static_cast<int>(_houseTexture.getSize().x))
                    textureX =
                    _houseTexture.getSize().x - 1;

                if (textureY < 0)
                    textureY = 0;

                if (textureY >=
                    static_cast<int>(_houseTexture.getSize().y))
                    textureY =
                    _houseTexture.getSize().y - 1;

                pixelColor =
                    _houseTexture.getPixel(
                        textureX,
                        textureY
                    );

                // Make the side walls darker
                if (side == 1)
                {
                    pixelColor.r *= 0.7f;
                    pixelColor.g *= 0.7f;
                    pixelColor.b *= 0.7f;
                }
            }
            else
            {
                pixelColor = wallColor;
            }

            _renderer.setPixel(
                ray,
                y,
                pixelColor
            );
        }


        rayAngle += angleStep;
    }
}