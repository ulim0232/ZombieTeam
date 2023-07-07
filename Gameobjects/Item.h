#pragma once
#include "SpriteGo.h"
class Item : public SpriteGo
{
protected:

	bool isSpawn = false;

public:
	enum class ItemTypes
	{
		Ammo,
		Potion,
	};

	Item(const string& textureId = "", const string& n = "");
	virtual ~Item() override;

	void SetType(ItemTypes t);
	ItemTypes GetType() const;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
};

