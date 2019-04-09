#pragma once

//���ø��� ������ ��������̼� �ؾ��Ѵ�?

template <typename T>
class TemplateSingleton
{
public:
	TemplateSingleton() {};
	virtual ~TemplateSingleton() {};

public:
	static T * GetInstance()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new T;
		}
		return m_pInstance;
	}

	static void DestroyInstance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static T * m_pInstance;
};

template<typename T> T * TemplateSingleton<T>::m_pInstance = nullptr;