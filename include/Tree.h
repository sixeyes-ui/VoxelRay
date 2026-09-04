#ifndef TREE_H
#define TREE_H

#include <SFML/System.hpp>

class Tree
{
public:
	Tree(float x, float y);

	sf::Vector2f getPosition() const;

private:
	sf::Vector2f _position;
};

#endif // !TREE_H
