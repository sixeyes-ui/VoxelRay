#include "include/World.h"
#include "include/Map.h"

World::World()
{
    Map::init();

    const std::vector<std::string>& map = Map::getMap();

    for (int y = 0; y < static_cast<int>(map.size()); y++)
    {
        for (int x = 0; x < static_cast<int>(map[y].size()); x++)
        {
            if (map[y][x] == 'T')
            {
                float worldX = x * Map::TILE_SIZE + Map::TILE_SIZE / 2.0f;
                float worldY = y * Map::TILE_SIZE + Map::TILE_SIZE / 2.0f;

                _trees.emplace_back(worldX, worldY);
            }
        }
    }

    for (int y = 0; y < static_cast<int>(map.size()); y++)
    {
        for (int x = 0; x < static_cast<int>(map[y].size()); x++)
        {
            if (map[y][x] == 'R')
            {
                float worldX = x * Map::TILE_SIZE + Map::TILE_SIZE / 2.0f;
                float worldY = y * Map::TILE_SIZE + Map::TILE_SIZE / 2.0f;

                _rocks.emplace_back(worldX, worldY);
            }
        }
    }

    for (int y = 0; y < static_cast<int>(map.size()); ++y)
    {
        for (int x = 0; x < static_cast<int>(map[y].size()); ++x)
        {
            // Make sure a 4x4 area fits inside the map
            if (x + 3 >= static_cast<int>(map[y].size()) ||
                y + 3 >= static_cast<int>(map.size()))
            {
                continue;
            }

            bool house =
                map[y][x] == 'H' &&
                map[y][x + 1] == 'H' &&
                map[y][x + 2] == 'H' &&
                map[y][x + 3] == 'H' &&

                map[y + 1][x] == 'H' &&
                map[y + 1][x + 1] == '.' &&
                map[y + 1][x + 2] == '.' &&
                map[y + 1][x + 3] == 'H' &&

                map[y + 2][x] == 'H' &&
                map[y + 2][x + 1] == '.' &&
                map[y + 2][x + 2] == '.' &&
                map[y + 2][x + 3] == 'H' &&

                map[y + 3][x] == 'H' &&
                map[y + 3][x + 1] == 'H' &&
                map[y + 3][x + 2] == 'H' &&
                map[y + 3][x + 3] == 'H';

            if (!house)
                continue;

            // Center of the 4x4 house
            float worldX =
                x * Map::TILE_SIZE +
                2.0f * Map::TILE_SIZE;

            float worldY =
                y * Map::TILE_SIZE +
                2.0f * Map::TILE_SIZE;

            _houses.emplace_back(
                worldX,
                worldY,
                4.0f * Map::TILE_SIZE,
                4.0f * Map::TILE_SIZE
            );
        }
    }
}

void World::update(float deltaTime)
{

}

const std::vector<Tree>& World::getTrees() const
{
    return _trees;
}

const std::vector<House>& World::getHouses() const
{
    return _houses;
}

const std::vector<Rock>& World::getRocks() const
{
    return _rocks;
}