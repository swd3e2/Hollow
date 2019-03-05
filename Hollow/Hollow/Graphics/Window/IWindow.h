#pragma once
#include "Hollow/Platform.h"

class HOLLOW_API IWindow
{
private:
	bool _isClosed;
protected:
	void SetIsClosed(bool status) { _isClosed = status; }
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
};