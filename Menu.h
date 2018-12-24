#pragma once
#include "ButtonsGroup.h"
#include "ButtonsGroupOnClickListener.h"

class Menu : public Drawable
{
private:
	Vector2f position;
protected:
	ButtonsGroup menu;
	ButtonsGroupOnClickListener *onClickListener;
public:
	Menu();
	void draw(RenderTarget& target, RenderStates states) const;
	void setPosition(Vector2f position);
};

