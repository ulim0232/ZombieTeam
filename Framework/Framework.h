#pragma once
#include "Singleton.h"
//singleton ���� ���, �̰��� �⺻ Ʋ
class Framework : public Singleton<Framework>
{
	friend Singleton<Framework>;
protected:
	Framework() = default;
	Framework(int w, int h, const string& t);
	virtual ~Framework() override = default;

	sf::RenderWindow window; //ȭ��
	sf::Clock clock; //�ð�

	int screenWidth = 1920;
	int screenHeight = 1080;
	string title = "Zombie Survival";

public:
	//ȭ���� ���� ����, Ÿ��Ʋ ȭ�鿡 �� �� ���ڷ� ����
	virtual void Init(int width, int height, const string& title);
	virtual void Release();

	virtual void Update(float dt);
	virtual void Draw(); //window�� ��� �����̱� ������ �Ű����� �ʿ�x
	virtual void Run(); //���� ���� ����
	
	sf::Vector2f GetWindowSize();
	sf::RenderWindow& GetWindow();
};

#define FRAMEWORK (Framework::Instance())
