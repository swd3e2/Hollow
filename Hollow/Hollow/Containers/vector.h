#pragma once
#include <iterator>
#include <assert.h>
#include <vector>
#define GROW 128

namespace Hollow { namespace Core { namespace Containers { 

	template<class T>
	class Vector
	{
	private:
		std::vector<T> kek;
		T* data;
		size_t size;
	public:
		Vector(size_t contianer_size = 1) : size(contianer_size)
		{
			this->data = new T[this->size];

			for (int i = 0; i < this->size; i++)
				this->data[i] = NULL;
		}

		void push_back(T& item)
		{
			push_back(std::move(item));
		}

		void push_back(T&& item)
		{
			if (this->size + 1 < this->size)
				this->resize(GROW);
				
			data[this->size++ - 1] = item;
		}

		Vector<T> resize(size_t new_size)
		{
			if (new_size > this->size) {
				T* temp = new T[new_size];
				memcpy((void*)temp, (void*)this->data, sizeof(T) * this->size);
				delete[] data;
				this->data = temp;
			}
			return *this;
		}

		T& operator[](int i)
		{
			assert(i < this->size);
			return this->data[i];
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
			inline bool operator==(iterator& other) const { return p == other.p; };
			inline bool operator!=(iterator& other) const { return p != other.p; };
			inline T& operator*() const { return *p; }
			inline T* operator->() const { return *p; }
		};

		iterator begin() { return iterator(this->data); }
		iterator end() { return iterator(this->data + this->size - 1); }
	};

} } }
