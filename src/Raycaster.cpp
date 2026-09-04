#include "../include/Raycaster.h"

#include "../include/Config.h"
#include "../include/Map.h"
#include "../include/Renderer.h"

#include <cmath>

Raycaster::Raycaster(Renderer& renderer)
    : _renderer(renderer)
{
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


        // =========================
        // DRAW WALL COLUMN
        // =========================

        sf::Color wallColor;

        if (side == 0)
        {
            wallColor =
                sf::Color(170, 100, 50);
        }
        else
        {
            // Darker side for fake lighting

            wallColor =
                sf::Color(120, 70, 35);
        }


        for (int y = wallTop;
            y < wallBottom;
            y++)
        {
            if (y >= 0 &&
                y < SCREEN_HEIGHT)
            {
                _renderer.setPixel(
                    ray,
                    y,
                    wallColor
                );
            }
        }


        rayAngle += angleStep;
    }
}