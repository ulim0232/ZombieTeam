#pragma once
#include "stdafx.h"

template <typename T>
class ObjectPool
{
protected:
	list<T*> pool; //��Ȱ��ȭ ����Ʈ
	list<T*> useList; //Ȱ��ȭ ����Ʈ
	
	void CreateObjs(int count = 100);

public:
	ObjectPool() = default;
	~ObjectPool();

	void Init(int cacheSize = 100); //�Ű������� �̸� ������ ���� ����
	void Release(); //new�� �� delete
	void Clear(); //pool�� ��ü ȸ��

	T* Get(); //pool���� ��Ȱ��ȭ�� �� �ϳ� ����
	void Return(T* obj); //Ȱ��ȭ�� �� ��Ȱ��ȭ

	const list<T*>& GetPool() const;
	const list<T*>& GetUseList() const;
	
	std::function <void(T*)> OnCreate; //��ȯ���� void�̰� �Ű������� T*�� �����Լ�
};

template<typename T>
inline void ObjectPool<T>::CreateObjs(int count)
{
	for (int i = 0; i < count; i++)
	{
		T* obj = new T();
		if (OnCreate != nullptr)
		{
			OnCreate(obj);
		}
		obj->Init();
		pool.push_back(obj);
	}
}

template<typename T>
inline ObjectPool<T>::~ObjectPool()
{
	Release();
}

template<typename T>
inline void ObjectPool<T>::Init(int cacheSize)
{
	//pool�� T���� cacheSize ��ŭ new-�����Ҵ�
	Release();
	CreateObjs(cacheSize);

}

template<typename T>
inline void ObjectPool<T>::Release()
{
	//pool, useList ���� �����Ҵ��� �� ��� ����-delete
	Clear();

	for(auto obj : pool)
	{
		delete obj;
	}
	pool.clear();
}

template<typename T>
inline void ObjectPool<T>::Clear()
{
	for (auto obj : useList)
	{
		obj->SetActive(false);
		pool.push_back(obj);
	}
	useList.clear();
}

template<typename T>
inline T* ObjectPool<T>::Get()
{
	//poo���� �ϳ� �̾Ƽ� Ȱ��ȭ �� �� ����
	if (pool.empty())
	{
		CreateObjs();
	}
	T* obj = pool.front();
	pool.pop_front();

	obj->SetActive(true);
	obj->Reset();

	useList.push_back(obj);
	return obj;

}

template<typename T>
inline void ObjectPool<T>::Return(T* obj)
{
	if (find(useList.begin(), useList.end(), obj) == useList.end())
	{
		cout << " Return error" << endl;
		return;
	}
	useList.remove(obj);
	obj->SetActive(false);
	pool.push_back(obj);
}

template<typename T>
inline const list<T*>& ObjectPool<T>::GetPool() const
{
	return pool;
}

template<typename T>
inline const list<T*>& ObjectPool<T>::GetUseList() const
{
	return useList;
}
