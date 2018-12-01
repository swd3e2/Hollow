#pragma once
#include <cstddef>
#include <limits>
#include "../../Platform.h"
#include <vector>
#include <algorithm>
#include <assert.h>

namespace ECS { namespace util {
	namespace Internal {

		template<typename handle_value_type, size_t version_bits, size_t index_bits>
		union Handle
		{
			using value_type = handle_value_type;


			static_assert(sizeof(value_type) * CHAR_BIT >= (version_bits + index_bits), "Invalid handle layout. More bits used than base value type can hold!");

			static constexpr size_t NUM_VERSION_BITS{version_bits};
			static constexpr size_t NUM_INDEX_BITS{index_bits};

			static constexpr value_type MIN_VERSION{0};
			static constexpr value_type MAX_VERSION{ ( 1U << NUM_VERSION_BITS ) - 2U };
			static constexpr value_type MAX_INDICES{ (1U << NUM_INDEX_BITS) - 2U };

			static constexpr value_type INVALID_HANDLE{std::numeric_limits<value_type>::max()};
		private:
			value_type value;
		public:
			struct 
			{
				value_type index		: NUM_INDEX_BITS;
				value_type version	: NUM_VERSION_BITS;
			};
			Handle() {}

			Handle(value_type value) :
				value(value)
			{}

			Handle(value_type index, value_type version) :
				index(index),
				version(version)
			{}

			inline operator value_type() const { return value; }
		};
	}

	using Handle32 = Internal::Handle<u32, 12, 20>;

#ifdef ECS_64BIT
	using Handle64 = Internal::Handle<u64, 24, 40>;
#else
	using Handle64 = Handle32;
#endif

	template<class T, class handle_type, size_t grow = 1024>
	class HandleTable 
	{
	private:
		std::vector<std::pair<typename handle_type::value_type, T*>> m_Table;

		void GrowTable()
		{
			size_t oldSize = this->m_Table.size();
			assert(oldSize < handle_type::MAX_INDICES && "Max table capacity reached!");
			size_t newSize = std::min(oldSize + grow, handle_type::MAX_INDICES);
			this->m_Table.resize(newSize);
			for (typename handle_type::value_type i = oldSize; i < newSize; ++i)
				this->m_Table[i] = std::pair<typename handle_type::value_type, T*>(handle_type::MIN_VERSION, nullptr);
		}

	public:
		HandleTable()
		{
			this->GrowTable();
		}

		~HandleTable()
		{}

		handle_type AqcuireHandle(T* rawObject)
		{
			typename handle_type::value_type i = 0;
			for (; i < this->m_Table.size(); ++i)
			{
				if (this->m_Table[i].second == nullptr)
				{
					this->m_Table[i].second = rawObject;
					this->m_Table[i].first = ((this->m_Table[i].first + 1) > handle_type::MAX_VERSION) ? handle_type::MIN_VERSION : this->m_Table[i].first + 1;
					return handle_type(i, this->m_Table[i].first);
				}
			}

			this->GrowTable();

			this->m_Table[i].first = 1;
			this->m_Table[i].second = rawObject;

			return handle_type(i, this->m_Table[i].first);
		}

		void ReleaseHandle(handle_type handle)
		{
			assert((handle.index < this->m_Table.size() && handle.version == this->m_Table[handle.index].first) && "Invalid handle!");
			this->m_Table[handle.index].second = nullptr;
		}

		inline bool CheckExpired(handle_type handle) const
		{
			return this->m_Table[handle.index].first != handle.version;
		}
		
		inline handle_type operator[](typename handle_type::value_type index) const
		{
			assert(index < this->m_Table.size() && "Invalid handle!");
			return handle_type(index, this->m_Table[index].first);
		}

		inline T* operator[](handle_type handle)
		{
			assert((handle.index < this->m_Table.size() && handle.version == this->m_Table[handle.index].first) && "Invalid handle!");
			return (this->m_Table[handle.index].first == handle.version ? this->m_Table[handle.index].second : nullptr);
		}
	};
}}