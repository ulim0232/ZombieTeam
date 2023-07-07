#pragma once
#include "Scene.h"
#include "ObjectPool.h"

class Player;
class VertexArrayGo;
class Zombie;
class SpriteEffect;

class SceneDev1 :public Scene
{
protected:
	Player* player;
	
	VertexArrayGo* background;
	sf::FloatRect wallBounds;

	ObjectPool<Zombie> zombiePool;
	ObjectPool<SpriteEffect> bloodEffectPool;

	bool isGameOver;
	int score = 0;
	int HiScore = 0;
	int wave = 0;
	bool isPause = true;

public:
	SceneDev1();
	virtual ~SceneDev1() override;
	virtual void Init() override;
	virtual void Release() override;

	//���� ����ϰ� ������ �� ȣ���� �Լ�
	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	VertexArrayGo* CreateBackGround(sf::Vector2i size, sf::Vector2f tileSize, sf::Vector2f texSize, string textureId);
	
	void SpawnZombies(int count, sf::Vector2f center, float radius);

	template <typename T>
	void ClearObjectPool(ObjectPool<T>& pool);

	void OnDieZombie(Zombie* zombie); //���� �׾��� �� ������ �ؾ��� ��

	const list<Zombie*>* GetZombieList() const;

	void OnDiePlayer();
	VertexArrayGo* GetBackground();

};

template<typename T>
inline void SceneDev1::ClearObjectPool(ObjectPool<T>& pool)
{
	for (auto obj : pool.GetUseList())
	{
		RemoveGo(obj);
	}
	pool.Clear();
}
