#pragma once

#ifndef I_CORE_OBJECT_H
#define I_CORE_OBJECT_H

class ICoreObject
{
public:
	virtual void initialize() = 0;
	virtual void destroy() = 0;
};

#endif I_CORE_OBJECT_H