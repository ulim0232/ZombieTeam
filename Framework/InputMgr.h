#pragma once
#include "Singleton.h"

enum class Axis //��
{
	Horizontal, //������
	Vertical //������
};

struct AxisInfo //�� ���� �࿡ ���� ������ ����
{
	Axis axis;
	list<int> positivies;
	list<int> negatives;

	float sensi = 3.f; //���� �ٲ�� �ӵ� ����, Ŭ���� ������
	float value = 0.f; //�ش��ϴ� ���� ���� �Է°� -1.0~ 1.0 ������ ��
};

class InputMgr :public Singleton<InputMgr>
{
	friend Singleton<InputMgr>;
protected:
	InputMgr();
	virtual ~InputMgr() override = default;

	/*Ű���� �Է°� ���콺 �Է� ����Ʈ�� �ϳ��� ���.
	�պκ��� Ű����, �޺κ��� ���콺 �Է����� ���*/
	std::list<int> downList;
	std::list<int> upList;
	std::list<int> ingList;

	sf::Vector2f mousePos; //���콺 Ŀ�� ��ġ
	map<Axis, AxisInfo> axisInfoMap; //vector�� ����ϴ� ���� �� ������ map�� �����ϴ� �迡
	
public:
	void Update(float dt);
	void UpdateEvent(const sf::Event& ev);

	//Ű���� �Է�
	bool GetKeyDown(sf::Keyboard::Key key);
	bool GetKey(sf::Keyboard::Key key);
	bool GetKeyUp(sf::Keyboard::Key key);

	/*---���콺----*/
	const sf::Vector2f GetMousePos(); //���콺 ��ġ ��ǥ
	//���콺 �Է� ����
	bool GetMouseButtonDown(sf::Mouse::Button button);
	bool GetMouseButton(sf::Mouse::Button button);
	bool GetMouseButtonUp(sf::Mouse::Button button);



	//�� �Է� �˻�
	float GexAxis(Axis axis); //-1~1 ������ �Ǽ� ��ȯ
	float GexAxisRaw(Axis axis); //-1, 0 , 1 �� 1�� ��ȯ
};

#define INPUT_MGR (InputMgr::Instance())