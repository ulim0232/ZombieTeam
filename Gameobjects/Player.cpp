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
	poolBullets.OnCreate = [ptr](Bullet* bullet)//�����Լ�, ���ٽ�
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
	
	sf::Vector2f mousePos = INPUT_MGR.GetMousePos(); //���콺 ��ġ
	sf::Vector2f mouseWorldPos = SCENE_MGR.GetCurrScene()->ScreenToWorldPos(mousePos);
	sf::Vector2f playerScreenPos = SCENE_MGR.GetCurrScene()->WorldPosToScreen(position);

	/*----ȸ��, player�� ���콺�� �׻� �ٶ󺸰�----*/
	look = Utils::Normalize(mousePos - playerScreenPos);
	sprite.setRotation(Utils::Angle(look));

	/*----�̵�----*/
	//direction.x = INPUT_MGR.GexAxisRaw(Axis::Horizontal);
	//direction.y = INPUT_MGR.GexAxisRaw(Axis::Vertical);
	direction.x = INPUT_MGR.GexAxis(Axis::Horizontal);
	direction.y = INPUT_MGR.GexAxis(Axis::Vertical);

	//�뽬���
	//if (stamina > 0.f) //���¹̳ʰ� ������
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::LShift)&&stamina>0.f) //Ű�� ������
	{
		speed *= 1.5f; //���ǵ� ��!
	}
	// �뽬Ű�� ���������鼭 ����Ű�� ������ ���¹̳ʰ� �ְ�, ���¹̳�
	if ((DASH && (MOVERIGHT || MOVELEFT || MOVEUP || MOVEDOWN)) && (stamina > 0.f)) //Ű �������߿�
	{
		stamina -= useStamina * dt; //���¹̳� �������� ����

	}
	else
	{
		stamina += healStamina * dt;
		if (stamina > 10.f) //���¹̳� 10���� ����
		{
			stamina = maxStamina;
		}
	}

	
	//�뽬 ��
	if ((INPUT_MGR.GetKeyUp(sf::Keyboard::LShift) || stamina <= 0.f) && speed > 500.f) //���߿� ����
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

	/*---�Ѿ� �߻�---*/
	if (ammo > 0)
	{

		if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Left))
		{
			Bullet* bullet = poolBullets.Get();
			bullet->Fire(GetPosition(), look, 1000.f);

			Scene* scene = SCENE_MGR.GetCurrScene();
			SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene); //c++�� ����ȯ ������
			if (sceneDev1 != nullptr)
			{
				bullet->SetZombieList(sceneDev1->GetZombieList());
				sceneDev1->AddGo(bullet);
			}

			SCENE_MGR.GetCurrScene()->AddGo(bullet);
			ammo--;

			soundShoot.play(); //�ݹ� ����
			soundShoot.setVolume(5.f); //���� ����
		}
	}
	if (ammo <= 0) //�Ѿ��� �ٴڳ� ��� // ���������� �߰�
	{
		if (!reloadTimeCheck)
		{
			clock.restart();
			reloadTimeCheck = true;
		}
		
		if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Left)&& (clock.getElapsedTime() < reloadTime))
		{
			soundReloadFail.play(); //�ݹ� ����
			soundReloadFail.setVolume(50.f); //���� ����
		}
		if (clock.getElapsedTime() >= reloadTime)
		{
			
			
			//�Ѿ� ä���
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
void Player::OnHitZombie(int damage) //���񿡰� ���ݴ���.
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

void Player::OnDie() //�÷��̾ �׾��� ��
{
	isAlive = false;
	cout << "Player Die" << endl;

	Scene* scene = SCENE_MGR.GetCurrScene();
	SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene); //c++�� ����ȯ ������
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
