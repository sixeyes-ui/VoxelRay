#include "../include/HouseRenderer.h"

#include "../include/House.h"
#include "../include/Player.h"
#include "../include/Renderer.h"
#include "../include/Config.h"
#include "../include/Map.h"

#include <cmath>

HouseRenderer::HouseRenderer(Renderer& renderer)
    : _renderer(renderer)
{
}

void HouseRenderer::render(
    const House& house,
    const sf::Vector2f& camearPosition,
    float cameraAngle,
    const std::vector<float>& depthBuffer)
{
    sf::Vector2f playerPos = camearPosition;
    float playerAngle = cameraAngle;

    // =========================================
    // FUNCTION TO DRAW A WINDOW
    // =========================================

    auto drawWindow =
        [&](sf::Vector2f windowPos)
        {
            float dx =
                windowPos.x - playerPos.x;

            float dy =
                windowPos.y - playerPos.y;

            float distance =
                std::sqrt(dx * dx + dy * dy);

            if (distance < 1.0f)
                return;

            float angle =
                std::atan2(dy, dx);

            float angleDifference =
                angle - playerAngle;

            // Normalize angle
            while (angleDifference > PI)
                angleDifference -= 2.0f * PI;

            while (angleDifference < -PI)
                angleDifference += 2.0f * PI;

            // Outside FOV
            if (std::abs(angleDifference) > FOV / 2.0f)
                return;

            // Fisheye correction
            float correctedDistance =
                distance * std::cos(angleDifference);

            if (correctedDistance <= 0.1f)
                return;

            // =========================================
            // SCREEN POSITION
            // =========================================

            float screenX =
                (angleDifference + FOV / 2.0f)
                / FOV
                * SCREEN_WIDTH;

            int centerX =
                static_cast<int>(screenX);

            // =========================================
            // WINDOW SIZE
            // =========================================

            float windowHeight =
                house.getWindowHeight() *
                500.0f /
                correctedDistance;

            float windowWidth =
                house.getWindowWidth() *
                500.0f /
                correctedDistance;

            // =========================================
            // WINDOW VERTICAL POSITION
            // =========================================

            float wallHeight =
                (Map::TILE_SIZE * 500.0f) /
                correctedDistance;

            float wallBottom =
                SCREEN_HEIGHT / 2.0f +
                wallHeight / 2.0f;

            // Put window above the ground
            float bottom =
                SCREEN_HEIGHT / 2.0f + windowHeight * 0.3f;

            float top =
                bottom -
                windowHeight;

            float left =
                centerX -
                windowWidth / 2.0f;

            float right =
                centerX +
                windowWidth / 2.0f;

            // =========================================
            // DRAW WINDOW
            // =========================================

            for (int x =
                static_cast<int>(left);
                x <= static_cast<int>(right);
                ++x)
            {
                if (x < 0 || x >= SCREEN_WIDTH)
                    continue;

                // Don't draw through a closer wall
                if (correctedDistance >
                    depthBuffer[x] + 2.0f)
                {
                    continue;
                }

                for (int y =
                    static_cast<int>(top);
                    y < static_cast<int>(bottom);
                    ++y)
                {
                    if (y < 0 || y >= SCREEN_HEIGHT)
                        continue;

                    _renderer.setPixel(
                        x,
                        y,
                        sf::Color(40, 80, 100)
                    );
                }
            }
        };


    // =========================================
    // DRAW WINDOWS
    // =========================================

    drawWindow(house.getWindow1Position());

    drawWindow(house.getWindow2Position());


    // =========================================
    // DRAW DOOR
    // =========================================

    sf::Vector2f doorPos =
        house.getDoorPosition();

    float dx =
        doorPos.x - playerPos.x;

    float dy =
        doorPos.y - playerPos.y;

    float distance =
        std::sqrt(dx * dx + dy * dy);

    if (distance < 1.0f)
        return;

    float angle =
        std::atan2(dy, dx);

    float angleDifference =
        angle - playerAngle;

    while (angleDifference > PI)
        angleDifference -= 2.0f * PI;

    while (angleDifference < -PI)
        angleDifference += 2.0f * PI;

    if (std::abs(angleDifference) > FOV / 2.0f)
        return;

    float correctedDistance =
        distance * std::cos(angleDifference);

    if (correctedDistance <= 0.1f)
        return;

    float screenX =
        (angleDifference + FOV / 2.0f)
        / FOV
        * SCREEN_WIDTH;

    int centerX =
        static_cast<int>(screenX);

    float doorHeight =
        house.getDoorHeight() *
        500.0f /
        correctedDistance;

    float doorWidth =
        house.getDoorWidth() *
        500.0f /
        correctedDistance;

    float wallHeight =
        (Map::TILE_SIZE * 500.0f) /
        correctedDistance;

    float bottom =
        SCREEN_HEIGHT / 2.0f +
        wallHeight / 2.0f;

    float top =
        bottom -
        doorHeight;

    float left =
        centerX -
        doorWidth / 2.0f;

    float right =
        centerX +
        doorWidth / 2.0f;

    for (int x =
        static_cast<int>(left);
        x <= static_cast<int>(right);
        ++x)
    {
        if (x < 0 || x >= SCREEN_WIDTH)
            continue;

        if (correctedDistance >
            depthBuffer[x] + 2.0f)
        {
            continue;
        }

        for (int y =
            static_cast<int>(top);
            y < static_cast<int>(bottom);
            ++y)
        {
            if (y < 0 || y >= SCREEN_HEIGHT)
                continue;

            _renderer.setPixel(
                x,
                y,
                sf::Color(70, 40, 25)
            );
        }
    }
}