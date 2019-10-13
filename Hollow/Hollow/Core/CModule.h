#pragma once

#ifndef HW_CMODULE_H
#define HW_CMODULE_H

#include "Hollow/Platform.h"
#include <assert.h>
#include <typeinfo>
#include <utility>

namespace Hollow {
	/* Base class for all core main engine systems */
	template <typename T>
	class CModule
	{
	private:
		static T* _instance;
		static bool _startedUp;
		static bool _shutdown;
	protected:
		CModule() = default;
		virtual ~CModule() = default;

		CModule(const CModule&) = delete;
		CModule(CModule&&) = delete;
		CModule& operator=(CModule&&) = delete;
		CModule& operator=(const CModule&) = delete;
	public:
		static T* instance()
		{
			if (!isStartedUp()) {
				//Hollow::Log::GetCoreLogger()->critical("Trying to use module without starting it up, {}", typeid(T).name());
				assert(false && "Trying to use module without starting it up");
			}

			if (isShutdown()) {
				//Hollow::Log::GetCoreLogger()->critical("Module is shutdown, {}", typeid(T).name());
				assert(false && "Module is shutdown");
			}

			return _instance;
		}

		template<typename ...ARGS>
		static void startUp(ARGS&& ...args)
		{
			_instance = new T(std::forward(args)...);
			_startedUp = true;
			_instance->onStartUp();
		}

		template<class SubClass, typename ...Args>
		static void startUp(Args&& ...args)
		{
			_instance = (T*)(new SubClass(std::forward<Args>(args)...));
			_startedUp = true;
			_instance->onStartUp();
		}

		static void shutdown()
		{
			if (isShutdown()) {
				assert(false && "Module is already shutdown");
			}
			_instance->onShutdown();
			_shutdown = true;
			delete _instance;
		}
	protected:
		virtual void onStartUp() {}
		virtual void onShutdown() {}
		static bool isStartedUp() { return _startedUp; }
		static bool isShutdown() { return _shutdown; }
	};

	template<class T> T* CModule<T>::_instance = nullptr;
	template<class T> bool CModule<T>::_startedUp = false;
	template<class T> bool CModule<T>::_shutdown = false;
}

#endif