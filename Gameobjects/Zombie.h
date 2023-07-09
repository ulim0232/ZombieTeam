#pragma once
#include "SpriteGo.h"

class Player;
class Zombie : public SpriteGo
{
public:
	//���� ��������� ��ü ����
	enum class Types
	{
		Bloater,
		Chaser,
		Crawler
	};

	static const int TotalTypes = 3;
	static const string textureIds[3];
	static const float speedStats[3];
	static const int hpStats[3];
	static const int damages[3];
	static const float attackRates[3];
	static const int scores[3];
	
protected:
	Types zombieType;

	sf::Vector2f look; //�ٶ󺸴� ����
	sf::Vector2f direction; //�̵��ϴ� ����
	float speed = 0.f; //�̵� �ӵ�
	int maxHp = 0;
	int hp = 0;

	int damage = 0;
	float attackRate = 0.f; //���� �ֱ�
	float attackTimer = 0.f; //timer�� rate���� Ŀ���� ���� Ÿ�̹�
	int score = 0;

	Player* player = nullptr; //�÷��̾ �Ѿư����ϱ� ������ �ʿ�

public:

	Zombie(const string& n = "");
	virtual ~Zombie() override;

	virtual void Init() override; //���� �Ҵ�(������)
	virtual void Release() override; //���� �Ҵ� ����(�Ҹ���)
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetType(Types t);
	Types GetType() const;

	void SetPlayer(Player* p);

	void OnHitBullet(int damage); //�Ѿ˿� �¾��� ��
	int GetDamage();

	int GetScore();
};

