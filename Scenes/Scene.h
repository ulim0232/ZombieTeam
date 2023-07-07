#pragma once

class GameObject;
class Scene
{
protected:
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;

	SceneId sceneId;

	vector<tuple<ResourceTypes, string>> resources;
	list<GameObject*> gameObjects; 
	list<GameObject*> removeGameObjects; 

	//�ߺ��� ���� �ʵ��� ���� -> �Լ��� ���ؼ� �ְ� ����

	list<GameObject*> gameoObjects;
	sf::View worldView;
	sf::View uiView;

	sf::RenderWindow& window;

public:
	Scene(SceneId id = SceneId::None);
	virtual ~Scene();

	//���� �����ϰ� ������ �� ȣ���� �Լ�
	//��� ���� ������ ���� �ϱ�� ����
	virtual void Init() = 0;
	virtual void Release() = 0;

	//���� ����ϰ� ������ �� ȣ���� �Լ�
	virtual void Enter();
	virtual void Exit();

	virtual void Update(float dt);
	virtual void Draw(sf::RenderWindow& window);

	GameObject* FindGo(const string& name); 
	//�̸��� �ߺ��Ǹ� ���� ó���� �� ����

	void FindGo(list<GameObject*>& list, const string& name);
	//�̸��� �ش��ϴ� ��� ���ӿ�����Ʈ�� ����Ʈ�� ���

	bool Exist(GameObject* go);
	GameObject* AddGo(GameObject* go);
	void RemoveGo(GameObject* go);
	void SortGos(); //����

	sf::Vector2f ScreenToWorldPos(sf::Vector2f screenPos); //��ũ�� ��ǥ->������ǥ�� ��ȯ
	sf::Vector2f ScreenToUiPos(sf::Vector2f screenPos); //��ũ��->ui��ǥ�� ��ȯ

	sf::Vector2f WorldPosToScreen(sf::Vector2f worldPos); //��ũ�� ��ǥ->������ǥ�� ��ȯ
	sf::Vector2f UiPosToScreen(sf::Vector2f uiPos);
};

