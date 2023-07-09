#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"
#include "Bullet.h"
#define DASH INPUT_MGR.GetKey(sf::Keyboard::LShift) 
#define MOVEUP INPUT_MGR.GetKey(sf::Keyboard::W) 
#define MOVERIGHT INPUT_MGR.GetKey(sf::Keyboard::D) 
#define MOVELEFT INPUT_MGR.GetKey(sf::Keyboard::A) 
#define MOVEDOWN INPUT_MGR.GetKey(sf::Keyboard::S)

class Zombie;

class Player : public SpriteGo
{
protected:
	sf::Vector2f look; //�ٶ󺸴� ����
	sf::Vector2f direction;
	float speed;
	int maxHp = 100;
	int hp = 0;
	int maxAmmo = 5;
	int ammo = 0;
	//���¹̳� �߰�
	float maxStamina = 10.f;
	float stamina = 0.f;
	float useStamina = 5.f;
	float healStamina = 2.f;

	float CollisionTimer = 0.0f; 
	float Interval = 1.f; //ü�� ���� ����

	sf::FloatRect wallBounds;
	sf::Vector2f wallBoundsLT;
	sf::Vector2f wallBoundsRB;

	// ���� �߰�
	sf::SoundBuffer soundBufferShoot;
	sf::SoundBuffer soundBufferHit;
	sf::SoundBuffer soundBufferReloadFail;
	sf::SoundBuffer soundBufferReload;

	sf::Sound soundShoot;
	sf::Sound soundHit;
	sf::Sound soundReloadFail;
	sf::Sound soundReload;



	sf::Clock clock; //����ð��� �����ϴ� ��ü
	const sf::Time reloadTime = sf::seconds(3.f); //3�� ���� ����
	bool reloadTimeCheck = false;
	
	

	//�߰� ��

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
	int GetMaxHp();
	float GetStamina();
	float GetMaxStamina();
	bool isAlive = false;
};

