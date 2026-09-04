#include "include/Tree.h"

Tree::Tree(float x, float y)
	: _position(x, y)
{
}

sf::Vector2f Tree::getPosition() const
{
	return _position;
}