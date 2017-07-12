//You Oyadomari
//Kokusai Denshi Business College
//Initial 2017/5/6
//LastUpdate 2017/5/6

#pragma once
#include <Windows.h>


template<class T>
//singleton class
//if inherit this class
//,register friend class
//,set constructor, copy constructor and "=" operator to private function
//,set destructor protected function and virtual function
class Singleton
{
private:
	static T* instance;	//instance

	void operator = (const Singleton &obj){}	//prohibit substitution operator
	Singleton(const Singleton &obj){}			//prohibit copy constructor

protected:
	Singleton(void){}				//prohibit create instance out of this class
	virtual ~Singleton(void){}	//virtualize destructor

public:
	static T* Create()
	{
		if(instance == NULL)
			instance = new T;
		return instance;
	}

	static T* Destroy()
	{
		if(instance != NULL)
		{
			delete instance;
			instance = NULL;
		}
		return instance;
	}

	static T* GetInstance()
	{
		return instance;
	}

	static T& GetInstanceRef()
	{
		return *instance;
	}
};

//initialize
template <class T>
T* Singleton<T>::instance = NULL;