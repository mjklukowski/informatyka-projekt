#include "Game.h"
#include "Entity.h"
#include "NoFlagException.h"

map<string, Texture*> Entity::textures;

void Entity::handleGravity(BlocksVector &blocks, float gravity)
{
	if (!isActive())
		return;

	if (yVelocityDown == yVelocityUp)
		yVelocityDown = 0.0;

	if (!canGoUp(blocks))
	{
		yVelocityUp = 0.0;
		yVelocityDown = 0.0;
		jumping = false;
		setMovingDirectionY(0);
	}

	yVelocityDown += gravity * 0.0017;
	move(Vector2f(0, (yVelocityDown + yVelocityUp) * Block::WIDTH));
	if (yVelocityDown + yVelocityUp > 0)
		setMovingDirectionY(1);
	else if (yVelocityDown + yVelocityUp < 0)
		setMovingDirectionY(-1);

	Vector2f entityPosition = getPosition();
	float eX = entityPosition.x;
	float eY = entityPosition.y;

	if (!canGoDown(blocks))
	{
		Sprite::setPosition(Vector2f(eX, (int)(eY / Block::WIDTH) * Block::WIDTH));
		yVelocityDown = 0.0;
		yVelocityUp = 0.0;
		setMovingDirectionY(0);
		return;
	}

	if (entityPosition.y > Game::HEIGHT + WIDTH)
		alive = false;
}

bool Entity::canGoRight(BlocksVector &blocks)
{
	Vector2f entityPosition = getPosition();
	float eX = entityPosition.x;
	float eY = entityPosition.y;

	Block *blockU = blocks.getSolidBlockAtPosition((eX - WIDTH / 2 + Block::WIDTH) / Block::WIDTH, (eY - WIDTH) / Block::WIDTH);
	Block *blockD = blocks.getSolidBlockAtPosition((eX - WIDTH / 2 + Block::WIDTH) / Block::WIDTH, (eY - 1) / Block::WIDTH);

	if (getFlag(Flags::SMART))
	{
		Block *blockD2 = blocks.getSolidBlockAtPosition((eX) / Block::WIDTH, (eY + WIDTH) / Block::WIDTH - 1);
		if (blockD2 == NULL && !isMovingY)
			return false;
	}
	if (blockU == NULL && blockD == NULL)
		return true;

	return false;
}

bool Entity::canGoLeft(BlocksVector &blocks)
{
	Vector2f entityPosition = getPosition();
	float eX = entityPosition.x;
	float eY = entityPosition.y;

	Block *blockU = blocks.getSolidBlockAtPosition((eX + WIDTH / 2 - Block::WIDTH - 4) / Block::WIDTH, (eY - WIDTH) / Block::WIDTH);	// - 4 (pr�dko�� w piks)
	Block *blockD = blocks.getSolidBlockAtPosition((eX + WIDTH / 2 - Block::WIDTH - 4) / Block::WIDTH, (eY - 1) / Block::WIDTH);		// - 4 (pr�dko�� w piks)

	if (getFlag(Flags::SMART))
	{
		Block *blockD2 = blocks.getSolidBlockAtPosition((eX - 1) / Block::WIDTH, (eY + WIDTH) / Block::WIDTH  - 1);
		if (blockD2 == NULL && !isMovingY)
			return false;
	}
	if (blockU == NULL && blockD == NULL)
		return true;

	return false;
}

bool Entity::canGoUp(BlocksVector & blocks)
{
	Vector2f entityPosition = getPosition();
	float eX = entityPosition.x;
	float eY = entityPosition.y;

	Block *blockUL = blocks.getSolidBlockAtPosition((eX - WIDTH / 2) / Block::WIDTH, (eY - WIDTH + ((yVelocityDown + yVelocityUp) + 0.025) * Block::WIDTH) / Block::WIDTH);
	Block *blockUR = blocks.getSolidBlockAtPosition((eX + WIDTH / 2 - 1) / Block::WIDTH, (eY - WIDTH + ((yVelocityDown + yVelocityUp) + 0.025) * Block::WIDTH) / Block::WIDTH);

	if (blockUL == NULL && blockUR == NULL)
		return true;

	return false;
}

bool Entity::canGoDown(BlocksVector & blocks)
{
	Vector2f entityPosition = getPosition();
	float eX = entityPosition.x;
	float eY = entityPosition.y;

	Block *blockDL = blocks.getSolidBlockAtPosition((eX - WIDTH / 2) / Block::WIDTH, (eY - WIDTH + Block::WIDTH) / Block::WIDTH);
	Block *blockDR = blocks.getSolidBlockAtPosition((eX + WIDTH / 2 - 1) / Block::WIDTH, (eY - WIDTH + Block::WIDTH) / Block::WIDTH);

	if (blockDL == NULL && blockDR == NULL)
		return true;

	return false;
}

void Entity::handleMovement(BlocksVector &solidBlocks)
{

	if (!isActive())
		return;

	if (!isAlive())
		return;

	if (getMovingDirectionX() == 0)
		setMovingDirectionX(-1);

	Vector2f velocity(0, 0);

	if (getMovingDirectionX() == -1)
	{
		if (canGoLeft(solidBlocks))
			velocity = Vector2f(-Block::WIDTH / 32, 0);
		else
			setMovingDirectionX(1);
	}
	else if (getMovingDirectionX() == 1)
	{
		if (canGoRight(solidBlocks))
			velocity = Vector2f(Block::WIDTH / 32, 0);
		else
			setMovingDirectionX(-1);
	}

	move(velocity);
}

void Entity::jump(double offset)
{
	if (!isActive())
		return;

	if (!isJumping() && yVelocityDown > 0)
		return;
	if (yVelocityUp == 0)
		yVelocityUp = -0.30 + offset;
	else if (yVelocityUp > -0.40 + offset)
		yVelocityUp -= 10 * 0.001;
	move(Vector2f(0, -0.025 * Block::WIDTH));
	jumping = true;
}

void Entity::setJumping(bool jumping)
{
	this->jumping = jumping;
}

bool Entity::isJumping()
{
	return jumping;
}

void Entity::animate()
{
	if (!isActive())
		return;

	Vector2u txtSize = texture->getSize();
	int deathFrame = (txtSize.x / WIDTH - 1);	//Ostatnia klatka tekstury przeznaczona na animacj� �mierci
	int jumpFrame = (txtSize.x / WIDTH - 2);	//Przedostatnia klatka tekstury przeznaczona na animacj� skoku

	if (!isAlive())
	{
		IntRect txtRect = getTextureRect();
		txtRect.left = deathFrame * WIDTH;
		setTextureRect(txtRect);
		return;
	}

	if (isMovingY) {
		IntRect txtRect = getTextureRect();
		txtRect.left = jumpFrame * WIDTH;
		if (isMovingX == 1)
			txtRect.top = 0;
		else if (isMovingX == -1)
			txtRect.top = WIDTH;
		setTextureRect(txtRect);
		return;
	}

	if (!isMovingX) {
		IntRect txtRect = getTextureRect();
		txtRect.left = 0;
		setTextureRect(txtRect);
		return;
	}

	if (animateClock.getElapsedTime().asMilliseconds() >= animationStep)
	{
		IntRect txtRect = getTextureRect();
		txtRect.left = WIDTH * ((txtRect.left / WIDTH) + 1);
		if (txtRect.left >= jumpFrame * WIDTH)
			txtRect.left = 0;
		if (isMovingX == 1)
			txtRect.top = 0;
		else if (isMovingX == -1)
			txtRect.top = WIDTH;
		setTextureRect(txtRect);
		animateClock.restart();
	}
}

void Entity::setMovingDirectionX(int x)
{
	isMovingX = x;
}

void Entity::setMovingDirectionY(int y)
{
	isMovingY = y;
}

int Entity::getMovingDirectionX()
{
	return isMovingX;
}

int Entity::getMovingDirectionY()
{
	return isMovingY;
}

bool Entity::getFlag(Flags flag)
{
	return flags[flag];
}

bool Entity::getFlag(string flag)
{
	return getFlag(getFlagByName(flag));
}

void Entity::setFlag(Flags flag, bool value)
{
	flags[flag] = value;
}

void Entity::setFlag(string flag, bool value)
{
	setFlag(getFlagByName(flag), value);
}

Entity::Flags Entity::getFlagByName(string name)
{
	if (name == "SMART")
		return Flags::SMART;
	else
		throw NoFlagException(name);
}

bool Entity::isActive()
{
	return active;
}

void Entity::activate()
{
	active = true;
}

void Entity::deactivate()
{
	active = false;
}

bool Entity::isAlive()
{
	return alive;
}

void Entity::die()
{
	alive = false;
}

bool Entity::isImmortal()
{
	return immortal;
}

const float Entity::getWidth() const
{
	return WIDTH;
}

void Entity::flushTextures()
{
	for (auto &pair : textures)
		delete pair.second;
	textures.clear();
}

void Entity::loadTexture(string texture)
{
	if (!textures.count(texture))
	{
		this->texture = new Texture();
		this->texture->loadFromFile(texture);
		textures.insert(pair<string, Texture*>(texture, this->texture));
	}
	else
		this->texture = textures.at(texture);
}

Entity::Entity()
{
	WIDTH = Block::WIDTH;
	setOrigin(Vector2f(WIDTH / 2, WIDTH));
	reset();
}

void Entity::setTexture(string texture)
{
	loadTexture(RES_DIR + texture);
	Sprite::setTexture(*this->texture);
	setTextureRect(IntRect(0, 0, WIDTH, WIDTH));
}

void Entity::reset()
{
	yVelocityDown = 0.0;
	yVelocityUp = 0.0;
	jumping = false;
	isMovingX = 0;
	isMovingY = 0;
	active = false;
	alive = true;
	immortal = false;
	setTextureRect(IntRect(0, WIDTH, WIDTH, WIDTH));
}

void Entity::setPosition(int x, int y)
{
	Sprite::setPosition(Vector2f(x * WIDTH + WIDTH / 2, y * WIDTH + WIDTH));
}