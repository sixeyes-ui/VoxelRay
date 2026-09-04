#include "include/Map.h"

std::vector<std::string> map;

void Map::init()
{
    map =
    {
        "####################",
        "#..................#",
        "#..................#",
        "#.....HHHH.........#",
        "#.....H..H.........#",
        "#.....H..H.........#",
        "#.....HHHH.........#",
        "#..................#",
        "#..................#",
        "#..................#",
        "#..........HHHH....#",
        "#..........H..H....#",
        "#..........HHHH....#",
        "#..................#",
        "#..................#",
        "####################"
    };
}

bool Map::isWall(int x, int y)
{
    if (y < 0 || y >= static_cast<int>(map.size()))
        return true;

    if (x < 0 || x >= static_cast<int>(map[y].size()))
        return true;

    return map[y][x] == '#' || map[y][x] == 'H';
}

const std::vector<std::string>& Map::getMap()
{
    return map;
}