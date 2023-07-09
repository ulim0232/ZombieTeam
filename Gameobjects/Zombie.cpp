#include "stdafx.h"
#include "Zombie.h"
#include "InputMgr.h"
#include "Player.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "SceneDev1.h"
#include "VertexArrayGo.h"

//���� ��������� ��ġ �Ǿ���ϴ� �κ� stakrt
const string Zombie::textureIds[3] = {
        "graphics/bloater.png",
        "graphics/chaser.png",
        "graphics/crawler.png"
};

const float Zombie::speedStats[3] = { 40.f, 70.f, 20.f };
const int Zombie::hpStats[3] = { 100, 75, 50 };
const int Zombie::damages[3] = { 15, 10, 5 };
const float Zombie::attackRates[3] = { 2.f, 0.5f, 1.f };
const int Zombie::scores[3] = { 10, 5, 3 };

Zombie::Zombie(const string& n)
    :SpriteGo("", n)
{
}

Zombie::~Zombie()
{
}

void Zombie::Init()
{
    SpriteGo::Init();
    SetOrigin(Origins::MC);
}

void Zombie::Release()
{
    SpriteGo::Release();
}

void Zombie::Reset()
{
    SpriteGo::Reset();

    hp = maxHp; //ü�� �ʱ�ȭ
    attackTimer = attackRate;
}

void Zombie::Update(float dt)
{
    SpriteGo::Update(dt);


    /*�÷��̾ �߰�*/
    if (player == nullptr)
        return;
    else
    {
        sf::Vector2f playerPos = player->GetPosition();

        float distance = Utils::Distance(playerPos, position);
        look = direction = Utils::Normalize(playerPos - position); //������-�� ��ġ: ���� ���� �� ����
        sprite.setRotation(Utils::Angle(look));

        if (distance > 25.f) //���� �Ÿ��� ��������� �������� ó��
        {
            position += direction * speed * dt;
            SetPosition(position);
        }
    }
    /*�÷��̾�� �浹*/
    attackTimer += dt;
    if(attackTimer > attackRate)
    {
        if (player->isAlive && sprite.getGlobalBounds().intersects(player->sprite.getGlobalBounds()))
        {
            attackTimer = 0.f;
            player->OnHitZombie(damage);
        }
    }
}

void Zombie::Draw(sf::RenderWindow& window)
{
    SpriteGo::Draw(window);
}

void Zombie::SetType(Types t)
{
    zombieType = t;

    int index = (int)zombieType;

    textureId = textureIds[index]; //�̹��� ����
    speed = speedStats[index]; //�ӵ� ����
    maxHp = hpStats[index]; //�ִ� ü�� ����
    damage = damages[index];
    attackRate = attackRates[index];
    score = scores[index];
}

Zombie::Types Zombie::GetType() const
{
    return zombieType;
}

void Zombie::SetPlayer(Player* p)
{
    player = p;
}

void Zombie::OnHitBullet(int damage)
{
    hp -= damage;
    if (hp <= 0) //hp�� 0�� �Ǹ� �״´�
    {
        Scene* scene = SCENE_MGR.GetCurrScene();
        SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene); //c++�� ����ȯ ������
        if (sceneDev1 != nullptr)
        {
            sceneDev1->OnDieZombie(this);
        }
    }
}

int Zombie::GetDamage()
{
    return damage;
}

int Zombie::GetScore()
{
    return score;
}
