#include "../include/Raycaster.h"

#include "../include/Config.h"
#include "../include/Map.h"
#include "../include/Renderer.h"

#include <cmath>

Raycaster::Raycaster(Renderer& renderer)
    : _renderer(renderer)
{
    _depthBuffer.resize(SCREEN_WIDTH);
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

        float distance;

        if (side == 0)
        {
            distance =
                sideDistX - deltaDistX;
        }
        else
        {
            distance =
                sideDistY - deltaDistY;
        }


        // =========================
        // FISHEYE CORRECTION
        // =========================

        float angleDifference =
            rayAngle - playerAngle;

        distance *= std::cos(angleDifference);


        // Avoid division by zero

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
            wallX = position.y + distance * rayDirY;
        }
        else
        {
            wallX = position.x + distance * rayDirX;
        }

        wallX /= Map::TILE_SIZE;

        wallX -= std::floor(wallX);

        // =========================
        // DRAW WALL COLUMN
        // =========================

        sf::Color wallColor;

        bool isHouse = Map::isHouse(mapX, mapY);

        if (isHouse)
        {
            if (side == 0)
            {
                wallColor = sf::Color(190, 120, 70);
            }
            else
            {
                wallColor = sf::Color(135, 80, 45);
            }
        }
        else
        {
            if (side == 0)
            {
                wallColor = sf::Color(100, 100, 100);
            }
            else
            {
                wallColor = sf::Color(70, 70, 70);
            }
        }


        for (int y = wallTop;
            y < wallBottom;
            y++)
        {
            if (y < 0 || y >= SCREEN_HEIGHT)
                continue;

            sf::Color pixelColor = wallColor;

            if (isHouse)
            {
                float wallY = static_cast<float>(y - wallTop) / static_cast<float>(wallBottom - wallTop);

                if (wallY < 0.18f)
                {
                    pixelColor = sf::Color(100, 45, 30);
                }

                if (wallX > 0.40f && wallX < 0.60f &&
                    wallY > 0.45f)
                {
                    pixelColor = sf::Color(80, 45, 25);
                }

                if (wallY > 0.25f && wallY < 0.55f)
                {
                    if ((wallX > 0.15f && wallX < 0.32f) ||
                        (wallX > 0.68f && wallX < 0.85f))
                    {
                        pixelColor = sf::Color(70, 150, 190);
                    }
                }
            }

            _renderer.setPixel(ray, y, pixelColor);
        }


        rayAngle += angleStep;
    }
}