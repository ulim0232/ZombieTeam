#include "stdafx.h"
#include "SpriteEffect.h"
#include "SceneMgr.h"
#include "Utils.h"


SpriteEffect::SpriteEffect(const string& textureId, const string& n)
	:SpriteGo(textureId, n)
{
}

void SpriteEffect::Init()
{
	SpriteGo::Init();
	SetOrigin(Origins::MC);
}

void SpriteEffect::Reset()
{
	SpriteGo::Reset();
	timer = 0.f;
	sprite.setColor({255, 255, 255, 255}); //������ �⺻���� ������ ,255�� �⺻��
}


void SpriteEffect::Update(float dt)
{
	timer += dt;

	//UINT8 a = ((duration - timer) / duration) * 255;
	
	//--float lerp ���--
	//UINT8 a = Utils::Lerp(0, 255, (duration - timer) / duration);
	//sprite.setColor({ 255, 255, 255, a });

	//--color lerpo ���---
	sf::Color a = { 255,255,255,0 };
	sf::Color b = sf::Color::White;
	sf::Color color = Utils::Lerp(a, b, (duration - timer) / duration);
	sprite.setColor(color); //���� ������������

	//--vecter2f lerp ���--
	//UINT8 a = Utils::Lerp(255, 0, (timer / duration));
	//SetPosition(Utils::Lerp(position, { 0, 0 }, (timer / duration), false));


	if (timer > duration) //���� �ð��� ������ ����
	{
		if (pool != nullptr)
		{
			SCENE_MGR.GetCurrScene()->RemoveGo(this);
			pool->Return(this);
		}
		else
		{
			SetActive(false);
		}
	}
}
