#pragma once

#ifndef ENTITY_H
#define ENTITY_H
#include "IEntity.h"

class EntityManager;

template<class T>
class Entity : public IEntity
{
	friend class EntityManager;
private:
	static size_t entityTypeId;
public:
	Entity() {}
	static size_t staticGetTypeId() { return entityTypeId; }
	size_t getTypeId() { return entityTypeId; }
};

template<class T> size_t Entity<T>::entityTypeId = typeid(Entity<T>).hash_code();

#endif