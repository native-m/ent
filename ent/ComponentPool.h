#pragma once

#include <vector>
#include <cassert>

namespace Hx
{
	class ComponentPoolBase
	{
	public:
		ComponentPoolBase(size_t componentSize, size_t chunkSize) :
			m_chunks(),
			m_componentSize(componentSize),
			m_chunkSize(chunkSize),
			m_size(0),
			m_capacity(0)
		{
		}

		virtual ~ComponentPoolBase()
		{
			for (auto& c : m_chunks)
			{
				delete[] c;
			}
		}

		inline void Resize(size_t newSize)
		{
			if (newSize >= m_size)
			{
				if (newSize >= m_capacity)
				{
					Reserve(newSize);
				}

				m_size = newSize;
			}
		}

		inline void Reserve(size_t size)
		{
			while (m_capacity < size)
			{
				m_chunks.push_back(new char[m_componentSize * m_chunkSize]);
				m_capacity += m_chunkSize;
			}
		}

		inline void* Get(size_t n)
		{
			assert(n < m_size && "Index exceeds the maximum size");
			return m_chunks[n / m_chunkSize] + m_componentSize * (n % m_chunkSize);
		}

		inline const void* Get(size_t n) const
		{
			assert(n < m_size && "Index exceeds the maximum size");
			return m_chunks[n / m_chunkSize] + m_componentSize * (n % m_chunkSize);
		}

		virtual void Destroy(size_t n) = 0;

	protected:
		std::vector<char*> m_chunks;
		size_t m_componentSize;
		size_t m_chunkSize;
		size_t m_size;
		size_t m_capacity;
	};

	template<class T, size_t ChunkSize = 1 << 13>
	class ComponentPool : public ComponentPoolBase
	{
	public:
		ComponentPool() :
			ComponentPoolBase(sizeof(T), ChunkSize)
		{
		}

		virtual ~ComponentPool()
		{
		}

		virtual void Destroy(size_t n) override
		{
			T* ptr = static_cast<T*>(Get(n));
			ptr->~T();
		}
	};
}