#pragma once
#include "Hollow/Platform.h"
#include <assert.h>

/* Base class for all core main engine systems */
template <typename T>
class CModule
{
private:
	static T* _instance;
	static bool _startedUp;
	static bool _shutdown;
protected:
	CModule()
	{
		_instance = static_cast<T*>(this);
	}
	virtual ~CModule() = default;

	CModule(const CModule&) = delete;
	CModule(CModule&&) = delete;
	CModule& operator=(CModule&&) = delete;
	CModule& operator=(const CModule&) = delete;
public:
	static T* instance()
	{
		if (!isStartedUp())
			assert(true && "Trying to create module without starting it up");

		if (isShutdown())
			assert(true && "Module is already shutdown");

		return _instance;
	}

protected:
	static bool isStartedUp()
	{
		return _startedUp;
	}

	static bool isShutdown()
	{
		return _shutdown;
	}
	inline void setStartedUp() { _startedUp = true; }
	inline void setShutdown() { _shutdown = true; }
};

template<class T> T* CModule<T>::_instance = nullptr;
template<class T> bool CModule<T>::_startedUp = false;
template<class T> bool CModule<T>::_shutdown = false;