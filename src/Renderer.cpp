#include "include/Renderer.h"

Renderer::Renderer(unsigned int width, unsigned int height) :
	_width(width),
	_height(height)
{
	_image.create(_width, _height, sf::Color::Black);
	_texture.loadFromImage(_image);
	_sprite.setTexture(_texture);
}

void Renderer::clear(sf::Color color)
{
	_image.create(_width, _height, color);
}

void Renderer::setPixel(unsigned int x, unsigned int y, sf::Color color)
{
	if (x >= _width || y >= _height)
		return;

	_image.setPixel(x, y, color);
}

void Renderer::display(sf::RenderWindow& window)
{
	_texture.update(_image);

	window.draw(_sprite);
}