#pragma once
#include "Hollow/Platform.h"

class HOLLOW_API IWindow
{
private:
	bool _isClosed;
protected:
	static IWindow* _instance;
public:
	IWindow() : 
		_isClosed(false)
	{
		if (_instance == nullptr)
			_instance = this;
	}

	static IWindow* instance() { return _instance; }
	
	bool isClosed() { return _isClosed; }
	void setIsClosed(bool status) { _isClosed = status; }
};