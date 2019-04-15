#ifndef ENTITY_H
#define ENTITY_H
#pragma once

class EntityManager;

template<class T>
class Entity
{
	friend class EntityManager;
private:
	size_t entityId;
	static size_t entityTypeId;
public:
	Entity(size_t entityId) : entityId(entityId) {}
	static size_t staticGetTypeId() { return entityTypeId; }
	size_t getTypeId() { return entityTypeId; }
};

template<class T> size_t Entity<T>::entityTypeId = typeid(Entity<T>).hash_code();

#endif