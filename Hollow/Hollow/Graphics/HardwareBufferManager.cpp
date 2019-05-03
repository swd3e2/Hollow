#include "HardwareBufferManager.h"

HardwareBufferManager::HardwareBufferManager()
{
	setStartedUp();
}

HardwareBufferManager::~HardwareBufferManager()
{
	setShutdown();
}
