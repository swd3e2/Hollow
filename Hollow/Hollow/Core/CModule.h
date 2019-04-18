#pragma once
#include "Hollow/Platform.h"
#include <assert.h>
#include <typeinfo>
#include "Hollow/Common/Log.h"

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
		if (!isStartedUp()) {
			Hollow::Log::GetCoreLogger()->critical("Trying to use module without starting it up, {}", typeid(T).name());
			assert(false && "Trying to use module without starting it up");
		}

		if (isShutdown()) {
			Hollow::Log::GetCoreLogger()->critical("Module is shutdown, {}", typeid(T).name());
			assert(false && "Module is shutdown");
		}

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