#ifndef WORLD_H
#define WORLD_H

#include <vector>

#include "Tree.h"
#include "House.h"
#include "Rock.h"

class World
{
public:
	World();

	void update(float deltaTime);

	const std::vector<Tree>& getTrees() const;
	const std::vector<House>& getHouses() const;
	const std::vector<Rock>& getRocks() const;

private:
	std::vector<Tree> _trees;
	std::vector<House> _houses;
	std::vector<Rock> _rocks;
};

#endif // !WORLD_H
