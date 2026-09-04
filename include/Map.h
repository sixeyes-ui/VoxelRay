#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>

class Map
{
public:
	static void init();

	static bool isWall(int x, int y);

	static const std::vector<std::string>& getMap();

public:
	static constexpr int TILE_SIZE = 64;
};

#endif // !MAP_H
