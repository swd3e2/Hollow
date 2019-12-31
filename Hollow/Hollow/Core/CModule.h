#pragma once

#ifndef HW_CMODULE_H
#define HW_CMODULE_H

#include "Hollow/Platform.h"
#include "Hollow/Common/Log.h"
#include <assert.h>
#include <typeinfo>
#include <utility>

namespace Hollow {
	/* Base class for all core engine systems */
	template <typename T>
	class CModule
	{
	private:
		static T* _instance;
		static bool _startedUp;
		static bool _shutdown;
	protected:
		CModule() = default;
		virtual ~CModule() { if (!isShutdown) shutdown(); }

		CModule(const CModule&) = delete;
		CModule(CModule&&) = delete;
		CModule& operator=(CModule&&) = delete;
		CModule& operator=(const CModule&) = delete;
	public:
		static T* instance()
		{
			if (!isStartedUp()) {
				HW_ERROR("Trying to use module without starting it up, {}", typeid(T).name());
				assert(false && "Trying to use module without starting it up");
			}

			if (isShutdown()) {
				HW_ERROR("Module is shutdown, {}", typeid(T).name());
				assert(false && "Module is shutdown");
			}

			return _instance;
		}

		template<typename ...ARGS>
		static T* startUp(ARGS&& ...args)
		{
#ifdef _DEBUG
			HW_INFO("Starting up {}", typeid(T).name());
#endif

			_instance = new T(std::forward(args)...);
			_startedUp = true;
			_shutdown = false;
			_instance->onStartUp();
			return _instance;
		}

		template<class SubClass, typename ...Args>
		static T* startUp(Args&& ...args)
		{
#ifdef _DEBUG
			HW_INFO("Starting up {}", typeid(SubClass).name());
#endif
			_instance = (T*)(new SubClass(std::forward<Args>(args)...));
			_startedUp = true;
			_shutdown = false;
			_instance->onStartUp();

			return _instance;
		}

		static void shutdown()
		{
#ifdef _DEBUG
			HW_INFO("Shuting down {}", typeid(T).name());
#endif
			if (isShutdown()) {
				assert(false && "Module is already shutdown");
			}
			_instance->onShutdown();
			_shutdown = true;
			_startedUp = false;
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