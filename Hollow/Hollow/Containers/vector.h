#pragma once
#include <iterator>
#include <assert.h>
#include <vector>
#include <iostream>

#define GROW_SIZE 128

namespace Hollow { namespace Containers { 

	template<class T, int grow_size = GROW_SIZE>
	class Vector
	{
	private:
		T* data;
		size_t capacity;
		bool is_primitive;
		size_t m_size;
	public:
		Vector() 
			: m_size(0), capacity(grow_size), is_primitive(std::is_integral<T>())
		{
			this->data = new T[this->capacity];
		}

		inline size_t size() { return this->size(); }
		void push_back(T& item)
		{
			push_back(std::move(item));
		}

		void push_back(T&& item)
		{
			if (this->m_size + 1 > this->capacity)
				this->resize(grow_size);

			// Size is -1 coz index of array starts from 0
			data[this->m_size++ - 1] = item;
		}

		void resize(size_t new_size)
		{
			if (new_size > this->capacity) {
				T* temp = new T[new_size];
				memcpy((void*)temp, (void*)this->data, sizeof(T) * this->m_size);
				delete[] data;
				this->data = temp;
			}
		}

		T& operator[](int i)
		{
			assert(i < this->m_size);
			return *(this->data + i);
		}

		void clear()
		{
			this->m_size = 0;
			/*if (!is_primitive) {
				for (int i = 0; i < this->size; i++) {
					this->data[i].~T();
					this->data[i] = nullptr;
				}
			}
			else {
				for (int i = 0; i < this->size; i++) {
					this->data[i] = nullptr;
				}
			}*/
		}

		class iterator : public std::iterator<std::forward_iterator_tag, T>
		{
		private:
			T* p;
		public:
			iterator(const iterator& it) : p(it.p) {}
			iterator(T *p) : p(p) {}

			inline iterator& operator++() 
			{
				++p;
				return *this;
			}

			inline iterator& operator--()
			{
				--p;
				return *this;
			}

			inline bool operator==(iterator& other) const { return p == other.p; };
			inline bool operator!=(iterator& other) const { return p != other.p; };
			inline T& operator*() const { return *p; }
			inline T* operator->() const { return *p; }
		};

		iterator begin() { return iterator(this->data); }
		iterator end() { return iterator(this->data + this->m_size - 1); }
	};

} }
