#include "LevelEndScreen.h"
#include "Game.h"

LevelEndScreen::LevelEndScreen()
{
	overlay.setFillColor(Color(0, 0, 0, 192));
	overlay.setSize(Vector2f(Game::WIDTH, Game::HEIGHT));

	container.setFillColor(Color::Blue);
	container.setOrigin(Vector2f(Game::WIDTH * 0.8 / 2, Game::HEIGHT * 0.5 / 2));
	container.setSize(Vector2f(Game::WIDTH * 0.8, Game::HEIGHT * 0.5));

	headerFont.loadFromFile("resources/fonts/verdana.ttf");

	header.setFont(headerFont);
	setPosition(Vector2f(0, 0));
}

void LevelEndScreen::draw(RenderWindow & window)
{
	window.draw(overlay);
	window.draw(container);
	window.draw(header);
}

void LevelEndScreen::setPosition(Vector2f position)
{
	this->position = position;
	overlay.setPosition(Vector2f(position.x - Game::WIDTH / 2, position.y - Game::HEIGHT / 2));
	container.setPosition(Vector2f(position.x, position.y));
	header.setPosition(Vector2f(position.x, position.y - 120));
}

void LevelEndScreen::setHeader(string header)
{
	FloatRect gb;
	this->header.setString(header);
	gb = this->header.getGlobalBounds();
	this->header.setOrigin(Vector2f(gb.width / 2, gb.height / 2));
}