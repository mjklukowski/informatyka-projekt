#include "HealthBar.h"
#include "utils.h"

HealthBar::HealthBar()
{
	healthTexture.loadFromFile("resources/textures/hud/health.png");
	healthTextureEmpty.loadFromFile("resources/textures/hud/health_empty.png");
	setHealth(3);
	setPosition(Vector2f(0, 0));
}

void HealthBar::draw(RenderTarget& target, RenderStates states) const
{
	for (int i = 0; i < maxHealth; i++)
		target.draw(health[i]);
}

void HealthBar::setHealth(int hp)
{
	for (int i = 0; i < maxHealth; i++)
	{
		if (i < hp)
			health[i].setTexture(healthTexture);
		else
			health[i].setTexture(healthTextureEmpty);
	}
}

int HealthBar::getHealth()
{
	for (int i = 0; i < maxHealth; i++)
	{
		if (health[i].getTexture() == &healthTextureEmpty)
			return i;
	}
	return maxHealth;
}

void HealthBar::setMaxHealth(int maxHealth)
{
	this->maxHealth = maxHealth;
}

int HealthBar::getMaxHealth()
{
	return maxHealth;
}

void HealthBar::setPosition(Vector2f position)
{
	this->position = position;
	for (int i = 0; i < maxHealth; i++)
		health[i].setPosition(Vector2f(position.x + i * 50, position.y));
}

void HealthBar::move(Vector2f position)
{
	this->position.x += position.x;
	this->position.y += position.y;
	for (int i = 0; i < maxHealth; i++)
		health[i].move(position);
}

bool HealthBar::fadeIn(int duration)
{
	bool status = false;
	for(int i = 0; i < maxHealth; i++)
		status = Utils::fadeIn(health[i], duration);
	return status;
}

void HealthBar::hide()
{
	for (int i = 0; i < maxHealth; i++)
		health[i].setColor(Color(255,255,255,0));
}
