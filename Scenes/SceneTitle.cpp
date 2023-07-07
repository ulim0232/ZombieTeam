#include "stdafx.h"
#include "SceneTitle.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "ResourceMgr.h"
#include "SpriteGo.h"
#include "Framework.h"

#define BLOCKS 4
#define SQUARE 30.0


SceneTitle::SceneTitle()
	:Scene(SceneId::Title)
{
	screenSize = FRAMEWORK.GetWindowSize();
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/background.png"));
}

void SceneTitle::Init()
{
	Release();

	/*SpriteGo* Title = new SpriteGo("BackGround", "graphics/background.png");
	AddGo(Title);*/
	SpriteGo* Title = (SpriteGo*)AddGo(new SpriteGo("graphics/background.png"));
	Title->SetOrigin(Origins::TL);
	Title->SetPosition(0.f, 0.f);
	Title->sprite.setScale(0.55f, 1.f);
	AddGo(Title);

	for (auto go : gameObjects)
	{
		go->Init();
	}
}

void SceneTitle::Release()
{
	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;
	}
}

void SceneTitle::Enter()
{
	Scene::Enter();
	/*SpriteGo* Title = (SpriteGo*)FindGo("BackGround");
	Title->sprite.setTexture(*RESOURCE_MGR.GetTexture("graphics/background.png"));*/
}

void SceneTitle::Exit()
{
	Scene::Exit();
}

void SceneTitle::Update(float dt)
{
	Scene::Update(dt);

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Return))
	{
		SCENE_MGR.ChangeScene(SceneId::Dev1);
	}
}

void SceneTitle::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}