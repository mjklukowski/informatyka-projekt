#include "LevelFailedOnClickListener.h"
#include "Game.h"
#include <iostream>

using namespace std;


LevelFailedOnClickListener::LevelFailedOnClickListener()
{
}

void LevelFailedOnClickListener::onClick(int index)
{
	switch (index)
	{
		case MAIN_MENU:
		{
			Game::getInstance().backToMenu();
			break;
		}
		case RESTART:
		{
			Game::getInstance().getLevel().restart();
			break;
		}
	}
}