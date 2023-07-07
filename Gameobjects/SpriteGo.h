#pragma once
#include "GameObject.h"
class SpriteGo : public GameObject
{
protected:
	//sf::Sprite sprite; public���� �̵� in ��������
	//string textureId; //�ε��� �ؽ����� id
public:
	sf::Sprite sprite;
	string textureId; //�ε��� �ؽ����� id

	SpriteGo(const string& textureId = "", const string& n = "");
	virtual ~SpriteGo() override;

	virtual void SetPosition(const sf::Vector2f& p);
	virtual void SetPosition(float x, float y);

	virtual void SetOrigin(Origins origin);
	virtual void SetOrigin(float x, float y);

	virtual void Init() override; //���� �Ҵ�(������)
	virtual void Release() override; //���� �Ҵ� ����(�Ҹ���)
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
};

