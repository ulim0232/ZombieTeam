#include "stdafx.h"
#include "Item.h"

Item::Item(const string& textureId, const string& n)
	:SpriteGo(textureId, n)
{
}

Item::~Item()
{
}

void Item::SetType(ItemTypes t)
{
}

Item::ItemTypes Item::GetType() const
{
	return Item::ItemTypes();
}

void Item::Init()
{
	SpriteGo::Init();
}

void Item::Release()
{
	SpriteGo::Release();
}

void Item::Reset()
{
	SpriteGo::Reset();
}

void Item::Update(float dt)
{
	SpriteGo::Update(dt);
}

void Item::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}
