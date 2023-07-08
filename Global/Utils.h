#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include "Defines.h"


class Utils
{
private:
	static std::random_device rd;
	static std::mt19937 gen;

public:
	static int RandomRange(int min, int maxExclude);
	static float RandomRange(float min, float max);

	static sf::Vector2f RandomOnCircle(float radius);
	static sf::Vector2f RandomInCircle(float radius);
	static float RandomValue(); //0.0f~1.0f �� ����

	static void SetOrigin(sf::Sprite& sprite, Origins origin);
	static void SetOrigin(sf::Text& text, Origins origin);
	static void SetOrigin(sf::Shape& shape, Origins origin);
	static void SetOrigin(sf::Transformable& obj, Origins origin, const sf::FloatRect& rect);

	/*---Ŭ���� ����---*/
	static float Clamp(float v, float min, float max);
	static sf::Vector2f Clamp(const sf::Vector2f& v, const sf::Vector2f& min, const sf::Vector2f& max);

	//��������
	static float Lerp(float min, float max, float t, bool clamping = true);
	static sf::Vector2f Lerp(const sf::Vector2f& min, const sf::Vector2f& max, float t, bool clamping = true);
	static sf::Color Lerp(const sf::Color& min, const sf::Color& max, const float t, bool clamping = true);

	/*----���� ���� �Լ�----*/
	static float SqrMagnitude(const sf::Vector2f& vec); //�纯 ������ ��
	static float Magnitude(const sf::Vector2f& vec); //�纯 ������ ���� ��Ʈ = ������ ����

	//����ȭ, �븻������
	static sf::Vector2f Normalize(const sf::Vector2f& vec);

	//2���� ������ ������ ���̸� ���ϴ� �Լ�
	static float Distance(const sf::Vector2f& pos1, const sf::Vector2f& pos2);

	//���̰��� ���ϴ� �Լ�
	//��κ��� ���� ����(180���� ����)�̹Ƿ� �̸� degree ������ ��ȯ�� �� ��ȯ
	static float Angle(const sf::Vector2f& dir); 
	static float Angle(const sf::Vector2f& start, const sf::Vector2f& end);
	
	static sf::Vector2f Direction(const float angle);
};



