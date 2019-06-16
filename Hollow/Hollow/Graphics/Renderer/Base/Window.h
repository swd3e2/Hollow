#pragma once

#ifndef HW_WINDOW_H
#define HW_WINDOW_H

#include "Hollow/Platform.h"
#include "Hollow/Core/CModule.h"

namespace Hollow {
	class Window : public CModule<Window>
	{
	private:
		bool _isClosed;
	protected:
	public:
		Window() :
			_isClosed(false)
		{}

		bool isClosed() { return _isClosed; }
		void setIsClosed(bool status) { _isClosed = status; }
		virtual bool ProcessMessage() = 0;
	};
}

#endif