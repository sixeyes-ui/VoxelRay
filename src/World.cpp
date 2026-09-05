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
            if (map[y][x] == 'H')
            {
                float worldX =
                    x * Map::TILE_SIZE +
                    Map::TILE_SIZE / 2.0f;

                float worldY =
                    y * Map::TILE_SIZE +
                    Map::TILE_SIZE / 2.0f;

                _houses.emplace_back(
                    worldX,
                    worldY,
                    Map::TILE_SIZE,
                    Map::TILE_SIZE
                );
            }
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