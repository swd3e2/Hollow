#pragma once

class IWindow
{
private:
	bool _isClosed;
protected:
	void SetIsClosed(bool status) { _isClosed = status; }
public:
	IWindow() : _isClosed(false) {}
	bool isClosed() { return _isClosed; }
};