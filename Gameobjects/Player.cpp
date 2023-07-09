#include "stdafx.h"
#include "Player.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "Bullet.h"
#include "SceneDev1.h"
#include "Zombie.h"
#include "VertexArrayGo.h"

Player::Player(const string textureId, const string n)
	:SpriteGo(textureId, n), speed(500.f)
{

}

void Player::Init()
{
	SpriteGo::Init();
	SetOrigin(Origins::MC);

	ObjectPool<Bullet>* ptr = &poolBullets;
	poolBullets.OnCreate = [ptr](Bullet* bullet)//무명함수, 람다식
	{
		bullet->textureId = "graphics/bullet.png";
		bullet->pool = ptr;
	};

	poolBullets.Init();
}

void Player::Release()
{
	SpriteGo::Release();

	poolBullets.Release();
}

void Player::Reset()
{
	SpriteGo::Reset();

	hp = maxHp;
	isAlive = true;
	ammo = maxAmmo;

	for (auto bullet : poolBullets.GetUseList())
	{
		SCENE_MGR.GetCurrScene()->RemoveGo(bullet);
	}

	poolBullets.Clear();
}


void Player::Update(float dt)
{
	SpriteGo::Update(dt);
	
	sf::Vector2f mousePos = INPUT_MGR.GetMousePos(); //마우스 위치
	sf::Vector2f mouseWorldPos = SCENE_MGR.GetCurrScene()->ScreenToWorldPos(mousePos);
	sf::Vector2f playerScreenPos = SCENE_MGR.GetCurrScene()->WorldPosToScreen(position);

	/*----회전, player가 마우스를 항상 바라보게----*/
	look = Utils::Normalize(mousePos - playerScreenPos);
	sprite.setRotation(Utils::Angle(look));

	/*----이동----*/
	//direction.x = INPUT_MGR.GexAxisRaw(Axis::Horizontal);
	//direction.y = INPUT_MGR.GexAxisRaw(Axis::Vertical);
	direction.x = INPUT_MGR.GexAxis(Axis::Horizontal);
	direction.y = INPUT_MGR.GexAxis(Axis::Vertical);

	float magnitude = Utils::Magnitude(direction);
	if (magnitude > 1.f)
	{
		direction /= magnitude;
	}
	position += direction * speed * dt;
	

	if (!wallBounds.contains(position))
	{
		position = Utils::Clamp(position, wallBoundsLT, wallBoundsRB);
	}
	SetPosition(position);

	/*---총알 발사---*/
	if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Left))
	{
		Bullet* bullet = poolBullets.Get();
		bullet->Fire(GetPosition(), look, 1000.f);

		Scene* scene = SCENE_MGR.GetCurrScene();
		SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene); //c++의 형변환 연산자
		if (sceneDev1 != nullptr)
		{
			bullet->SetZombieList(sceneDev1->GetZombieList());
			sceneDev1->AddGo(bullet);
		}

		SCENE_MGR.GetCurrScene()->AddGo(bullet);
		ammo--;
	}
}

void Player::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}
void Player::OnHitZombie(int damage)
{
	if (!isAlive)
	{
		return;
	}
	hp = std::max(hp - damage, 0);
	cout << hp << endl;
	if (hp <= 0)
	{
		OnDie();
	}
}

void Player::TakeItem(Item::ItemTypes type)
{
	if (type == Item::ItemTypes::Ammo && ammo <= maxAmmo - 10)
	{
		ammo += 10;
	}
	if (type == Item::ItemTypes::Potion && hp <= maxHp - 10)
	{
		hp += 10;
	}
}

void Player::OnDie() //플레이어가 죽었을 때
{
	isAlive = false;
	cout << "Player Die" << endl;

	Scene* scene = SCENE_MGR.GetCurrScene();
	SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene); //c++의 형변환 연산자
	if (sceneDev1 != nullptr)
	{
		sceneDev1->OnDiePlayer();
	}
}

void Player::SetWallBounds(const sf::FloatRect& bounds)
{
	sf::Vector2f halfSize = { sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2 };

	wallBounds = bounds;

	wallBoundsLT = { wallBounds.left + halfSize.x, wallBounds.top + halfSize.y };
	wallBoundsRB = { wallBounds.left - halfSize.x + wallBounds.width, wallBounds.top - halfSize.y + wallBounds.height };
}

int Player::GetMaxAmmo()
{
	return maxAmmo;
}

int Player::GetAmmo()
{
	return ammo;
}

int Player::GetHp()
{
	return hp;
}
