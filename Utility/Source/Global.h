#pragma once

//�򵥿�ݵ�ȫ�ַ��ʷ�ʽ ��ֻ֧�ָ�����������һ��ָ��
template<typename T>
class GlobalVariable
{
public:

	inline static void Set(T* pVariable)
	{
		if (!m_pVariable) //ȫ��������ֻ�����ò����һ��
			m_pVariable = pVariable;
	}

	inline static T* Get()
	{
		return m_pVariable;
	}
	inline static T& Ref()
	{
		return *m_pVariable;
	}
	inline static T* Array(int index)
	{
		return m_pVariable + index;
	}

private:
	static T* m_pVariable;
};

template<class T>
inline T* GlobalVariable<T>::m_pVariable = nullptr;


