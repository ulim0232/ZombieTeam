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
	soundBufferShoot.loadFromFile("sound/shoot.wav");
	soundBufferHit.loadFromFile("sound/hit.wav");
	soundBufferReloadFail.loadFromFile("sound/reload_failed.wav");
	soundBufferReload.loadFromFile("sound/reload.wav");

	soundShoot.setBuffer(soundBufferShoot);
	soundHit.setBuffer(soundBufferHit);
	soundReloadFail.setBuffer(soundBufferReloadFail);
	soundReload.setBuffer(soundBufferReload);
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
	stamina = maxStamina;
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

	//대쉬기능
	//if (stamina > 0.f) //스태미너가 있을때
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::LShift)&&stamina>0.f) //키를 누르면
	{
		speed *= 1.5f; //스피드 업!
	}
	// 대쉬키가 눌려있으면서 방향키가 눌리고 스태미너가 있고, 스태미너
	if ((DASH && (MOVERIGHT || MOVELEFT || MOVEUP || MOVEDOWN)) && (stamina > 0.f)) //키 누르는중엔
	{
		stamina -= useStamina * dt; //스태미너 지속적인 감소

	}
	else
	{
		stamina += healStamina * dt;
		if (stamina > 10.f) //스태미너 10으로 고정
		{
			stamina = maxStamina;
		}
	}

	
	//대쉬 끝
	if ((INPUT_MGR.GetKeyUp(sf::Keyboard::LShift) || stamina <= 0.f) && speed > 500.f) //나중에 수정
	{
		speed /= 1.5f;
	}


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
	if (ammo > 0)
	{

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

			soundShoot.play(); //격발 사운드
			soundShoot.setVolume(5.f); //사운드 조정
		}
	}
	if (ammo <= 0) //총알이 바닥난 경우 // 장전딜레이 추가
	{
		if (!reloadTimeCheck)
		{
			clock.restart();
			reloadTimeCheck = true;
		}
		
		if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Left)&& (clock.getElapsedTime() < reloadTime))
		{
			soundReloadFail.play(); //격발 사운드
			soundReloadFail.setVolume(50.f); //사운드 조정
		}
		if (clock.getElapsedTime() >= reloadTime)
		{
			
			
			//총알 채우기
			ammo = maxAmmo;
			reloadTimeCheck = false;
			soundReload.play();
			soundReload.setVolume(5.f);
		}
	}
}

void Player::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}
void Player::OnHitZombie(int damage) //좀비에게 가격당함.
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
	else {
		soundHit.play();
		soundShoot.setVolume(3.f);
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

int Player::GetMaxHp()
{
	return maxHp;
}

float Player::GetStamina()
{
	return stamina;
}

float Player::GetMaxStamina()
{
	return maxStamina;
}
