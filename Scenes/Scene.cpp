#include "stdafx.h"
#include "Scene.h"
#include "GameObject.h"
#include "ResourceMgr.h"
#include "Framework.h"

Scene::Scene(SceneId id) : sceneId(id), window(FRAMEWORK.GetWindow())
{
}

Scene::~Scene()
{

}

//void Scene::Init()
//{
//	Release();
//
//	for (auto go : gameObjects)
//	{
//		go->Init();
//	}
//}
//
//void Scene::Release()
//{
//	for (auto go : gameObjects)
//	{
//		//go->Release(); GeamObjects�� �Ҹ��ڿ��� release �ϴ� ������ ����
//		delete go;
//	}
//	gameObjects.clear();
//}

void Scene::Enter()
{
	RESOURCE_MGR.Load(resources);
	//������ ������ �ʱ� ���·� ����
	for (auto go : gameObjects)
	{
		go->Reset();
	}
}

void Scene::Exit()
{
	for (auto go : removeGameObjects)
	{
		gameObjects.remove(go);
	}
	removeGameObjects.clear();

	RESOURCE_MGR.UnLoad(resources);
}

void Scene::Update(float dt)
{
	for (auto go : gameObjects)
	{
		if (go->GetActive()) //Ȱ��ȭ �� �� �� ������Ʈ
		{
			go->Update(dt);
		}
	}

	for (auto go : removeGameObjects)
	{
		gameObjects.remove(go);
	}
	removeGameObjects.clear();
}

void Scene::Draw(sf::RenderWindow& window)
{
	SortGos(); //draw ���� ���� �� �� �ϱ�
	window.setView(worldView);
	for (auto go : gameObjects)
	{
		if (go->sortLayer >= 100)
			//return;
			continue;

		if (go->GetActive()) //Ȱ��ȭ �� �� �� ������Ʈ
		{
			go->Draw(window);
		}
	}

	//ui�� sortLayer�� 100�̻�
	window.setView(uiView);
	for (auto go : gameObjects)
	{
		if (go->sortLayer < 100)
			//return;
			continue;

		if (go->GetActive()) //Ȱ��ȭ �� �� �� ������Ʈ
		{
			go->Draw(window);
		}
	}
}

GameObject* Scene::FindGo(const string& name)
{
	auto compare = [name](GameObject* go) {return go->GetName() == name; };
	auto it = find_if(gameObjects.begin(), gameObjects.end(), compare);
	if (it == gameObjects.end())
		return nullptr;
	return *it;

	/*���� ���� ǥ������ ����� �� �� ��1
	for (auto go : gameObjects)
	{
		if (go->GetName() == name)
		{
			return go;
		}
	}
	return nullptr;*/
}

void Scene::FindGo(list<GameObject*>& list, const string& name)
{
	//list.clear(); //����Ʈ�� ������ ��� �� ����, ���ÿɼ�
	for (auto go : gameObjects)
	{
		if (go->GetName() == name)
		{
			list.push_back(go);
		}
	}
}

bool Scene::Exist(GameObject* go)
{
	return find(gameObjects.begin(), gameObjects.end(), go) != gameObjects.end();
}

GameObject* Scene::AddGo(GameObject* go)
{
	if (!Exist(go)) //�������� �ʴ� ��쿡�� �ֱ�
	{
		gameObjects.push_back(go);
	}
	return go;
}

void Scene::RemoveGo(GameObject* go)
{
	removeGameObjects.push_back(go);
	//delete go; �ٱ����� �ϴ� ������ ����
}

void Scene::SortGos()
{
	gameObjects.sort([](GameObject* lhs, GameObject* rhs) {
		if (lhs->sortLayer != rhs->sortLayer)
			return lhs->sortLayer < rhs->sortLayer;
		return lhs->sortOrder< rhs->sortOrder; 
		});
	//����ǥ������ ����ؼ� ����
}

sf::Vector2f Scene::ScreenToWorldPos(sf::Vector2f screenPos)
{
	return window.mapPixelToCoords((sf::Vector2i)screenPos, worldView);
}

sf::Vector2f Scene::ScreenToUiPos(sf::Vector2f screenPos)
{
	return window.mapPixelToCoords((sf::Vector2i)screenPos, uiView);
}

sf::Vector2f Scene::WorldPosToScreen(sf::Vector2f worldPos)
{
	return (sf::Vector2f)window.mapCoordsToPixel(worldPos, worldView);
}

sf::Vector2f Scene::UiPosToScreen(sf::Vector2f uiPos)
{
	return (sf::Vector2f)window.mapCoordsToPixel(uiPos, uiView);
}
