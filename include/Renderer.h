#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics.hpp>

class Renderer
{
public:
	Renderer(unsigned int width, unsigned int height);

	void clear(sf::Color color);

	void setPixel(unsigned int x, unsigned int y, sf::Color color);

	void display(sf::RenderWindow& window);

private:
	sf::Image   _image;
	sf::Texture _texture;
	sf::Sprite  _sprite;

	unsigned int _width;
	unsigned int _height;
};

#endif // !RENDERER_H
