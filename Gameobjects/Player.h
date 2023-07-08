#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"
#include "Bullet.h"
class Zombie;

class Player : public SpriteGo
{
protected:
	sf::Vector2f look; //바라보는 방향
	sf::Vector2f direction;
	float speed;
	int maxHp = 100;
	int hp = 0;
	int maxAmmo = 900;
	int ammo = 0;
	float shotDelay = 0.5f;
	float timer = 0.f;
	bool rebound = false;
	int shotBulletNum = 1;


	float CollisionTimer = 0.0f; 
	float Interval = 1.f; //체력 깎을 간격

	sf::FloatRect wallBounds;
	sf::Vector2f wallBoundsLT;
	sf::Vector2f wallBoundsRB;

	ObjectPool<Bullet> poolBullets;
public:
	Player(const string textureId = "", const string n = "");

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetZombieList(list<Zombie*>* list);
	void OnHitZombie(int damage);

	void OnDie();

	void SetWallBounds(const sf::FloatRect& bounds);
	int GetMaxAmmo();
	int GetAmmo();
	int GetHp();

	void StatUp(int num);
	void DamageUp(int damage);
	void ShotSpeedUp(float shotDelay);
	void AddShotBullet();

	bool isAlive = false;
};

