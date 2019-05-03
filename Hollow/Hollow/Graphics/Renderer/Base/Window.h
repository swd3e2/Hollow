#pragma once

#include "Hollow/Platform.h"
#include "Hollow/Core/CModule.h"

class HOLLOW_API Window : public CModule<Window>
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