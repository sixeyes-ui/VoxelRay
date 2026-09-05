#ifndef WORLD_H
#define WORLD_H

#include <vector>

#include "Tree.h"
#include "House.h"

class World
{
public:
	World();

	void update(float deltaTime);

	const std::vector<Tree>& getTrees() const;
	const std::vector<House>& getHouses() const;

private:
	std::vector<Tree> _trees;
	std::vector<House> _houses;
};

#endif // !WORLD_H
