#include "stdafx.h"
#include "Utils.h"


std::random_device Utils::rd;
std::mt19937 Utils::gen(Utils::rd());

int Utils::RandomRange(int min, int maxExclude)
{
	std::uniform_int_distribution<> dist(min, maxExclude - 1);
	return dist(gen);
}

float Utils::RandomRange(float min, float max)
{
	std::uniform_real_distribution<> dist(min, max);
	return (float)dist(gen);
}

sf::Vector2f Utils::RandomOnCircle(float radius)
{
	sf::Vector2f vec(RandomRange(-1.0f, 1.0f), RandomRange(-1.0f, 1.0f)); //������ �������� ���μ��ΰ� 2�� ���簢��
	vec = Normalize(vec) * radius; //����ȭ, ������ �������� �������� 1�� ������ ���� �ݰ游ŭ ���ϱ�
	return vec;
}

sf::Vector2f Utils::RandomInCircle(float radius)
{
	return RandomOnCircle(radius) * RandomValue();
}

float Utils::RandomValue()
{
	return RandomRange(0.0f, 1.0f);
}

void Utils::SetOrigin(sf::Sprite& sprite, Origins origin)
{
	SetOrigin(sprite, origin, sprite.getLocalBounds());
}

void Utils::SetOrigin(sf::Text& text, Origins origin)
{
	SetOrigin(text, origin, text.getLocalBounds());
}

void Utils::SetOrigin(sf::Shape& shape, Origins origin)
{
	SetOrigin(shape, origin, shape.getLocalBounds());
}

void Utils::SetOrigin(sf::Transformable& obj, Origins origin, const sf::FloatRect& rect)
{
	sf::Vector2f originPos(rect.width, rect.height);
	originPos.x *= ((int)origin % 3) * 0.5f;
	originPos.y *= ((int)origin / 3) * 0.5f;
	obj.setOrigin(originPos);
}

float Utils::Clamp(float v, float min, float max)
{
	//ver1
	return std::max(min, std::min(max, v));


	/*ver2
	if (v < min)
	{
		return min;
	}
	if (v > max)
	{
		return max;
	}
	return v;*/
}

sf::Vector2f Utils::Clamp(const sf::Vector2f& v, const sf::Vector2f& min, const sf::Vector2f& max)
{
	sf::Vector2f result;
	result.x = Clamp(v.x, min.x, max.x);
	result.y = Clamp(v.y, min.y, max.y);

	return result;
}

float Utils::Lerp(float a, float b, float t, bool clamping)
{
	float v = a + (b - a) * t;
	if (clamping)
	{
		v = Utils::Clamp(v, std::min(a, b), std::max(a, b));
	}
	return v;
}

sf::Vector2f Utils::Lerp(const sf::Vector2f& a, const sf::Vector2f& b, float t, bool clamping)
{
	if (clamping)
	{
		t = Utils::Clamp(t, 0.f, 1.f);
	}
	return a + Utils::Normalize(b - a) * (Utils::Distance(a, b) * t);
}

sf::Color Utils::Lerp(const sf::Color& a, const sf::Color& b, const float t, bool clamping)
{
	sf::Color v;
	v.r = Lerp(a.r, b.r, t, clamping);
	v.g = Lerp(a.g, b.g, t, clamping);
	v.b = Lerp(a.b, b.b, t, clamping);
	v.a = Lerp(a.a, b.a, t, clamping);
	return v;
}

float Utils::SqrMagnitude(const sf::Vector2f& vec)
{
	return vec.x * vec.x + vec.y * vec.y;
}

float Utils::Magnitude(const sf::Vector2f& vec)
{
	return sqrt(vec.x * vec.x + vec.y * vec.y);
}

sf::Vector2f Utils::Normalize(const sf::Vector2f& vec)
//Ư���� ���� ������ ���� �ȿ� ����
//�Ű� ������ �Ѿ�� ������ ���̸� 1�� �����.
//����� ���� �� ���⸸ �ǹ��ְ� �ϴ� ��
{
	float mag = Magnitude(vec);
	if (mag == 0.f)
	{
		return vec;
	}
	return vec / mag;
}

float Utils::Distance(const sf::Vector2f& pos1, const sf::Vector2f& pos2)
{
	return Magnitude(pos2 - pos1);
}

float Utils::Angle(const sf::Vector2f& dir)
{
	return atan2(dir.y, dir.x) * (180.f / M_PI);
	//��ũ ź��Ʈ2 �Լ�, ���� ������ ����
	//m_pi�� ���̰�
}

float Utils::Angle(const sf::Vector2f& start, const sf::Vector2f& end)
{
	return Angle(Normalize(end - start)); 
	//start���� end�� �ٶ󺸴� ������ ���ؼ� �ѱ�
}

//������ �Ѱܼ� x, y��ǥ ���
sf::Vector2f Utils::Direction(const float angle)
{
	//PI�� ���� ���� / ���� �ѷ� == ������ / (���� �ѷ�/2) 
	//���� ������ �������� 1�� ���� ����
	float radians = angle * (M_PI / 180.f);

	// x�� y ��ǥ ���
	float x = cos(radians);
	float y = sin(radians);

	return sf::Vector2f(x, y);
}
