#pragma once

template<class T>
class ids
{
private:
	static int id;
public:
	template<class U>
	static int getId()
	{
		return id++;
	}
};

template<class T> int ids<T>::id = 0;