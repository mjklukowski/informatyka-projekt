#include "LevelEndScreen.h"
#include "Game.h"
#include "utils.h"

LevelEndScreen::LevelEndScreen()
{
	overlay.setFillColor(Color(0, 0, 0, 0));
	overlay.setSize(Vector2f(Game::WIDTH, Game::HEIGHT));
	
	frame.getContainer()->setTexture(*container.getTexture(), false);

	headerFont.loadFromFile("resources/fonts/verdana.ttf");
	header.setFont(Game::getInstance().getMainFont());

	menu.setSpacing(Vector2f(20, 0));

	container.append(header);
	container.append(menu);
	container.setBackground(Color(0, 0, 0, 96));

	setSize(Vector2f(1,1));
	setPosition(Vector2f(0, 0));
}

void LevelEndScreen::draw(RenderWindow & window)
{
	Utils::fadeIn(overlay, 100, 192);
	window.draw(overlay);

	container.draw();
	
	frame.setPosition(position);
	frame.showUp();
	window.draw(frame);
}

void LevelEndScreen::setPosition(Vector2f position)
{
	this->position = position;
	overlay.setPosition(Vector2f(position.x - Game::WIDTH / 2, position.y - Game::HEIGHT / 2));
	menu.setOffset(Vector2f(position.x - size.x / 2, position.y - size.y / 2));
}

void LevelEndScreen::setHeader(string header)
{
	FloatRect gb;
	this->header.setString(header);
	gb = this->header.getGlobalBounds();
	this->header.setOrigin(Vector2f(gb.width / 2, gb.height / 2));
}

void LevelEndScreen::setSize(Vector2f size)
{
	this->size = size;
	frame.setSize(size);
	container.setSize(size);
	header.setPosition(Vector2f(container.getSize().x / 2, 20));
	menu.setPosition(Vector2f(container.getSize().x / 2, container.getSize().y - 40));
}

Vector2f LevelEndScreen::getPosition()
{
	return position;
}
