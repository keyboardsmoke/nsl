/*
 * nsl::vector
 * Dynamically allocated list
 * Nowhere near as functional as `std::vector`
 * Author: Andrew Artz
*/

#pragma once

#include <limits>
#include "sort.h"

namespace nsl
{
	template<typename T, size_t SizeIncrement = 10>
	struct vector
	{
		// default constructor
		vector() noexcept : m_data(nullptr), m_lastUsedIdx(0), m_allocSize(0) { Reserve(SizeIncrement); }

		// copy constructor
		vector(vector<T> &other) noexcept { Copy(other); }

		// virtual for dynamic_cast
		virtual ~vector() noexcept { Reserve(0); }

		// index getter
		T& operator[](size_t idx) noexcept
		{
			return m_data[idx];
		}

		// index getter (const)
		const T& operator[](size_t idx) const noexcept
		{
			return m_data[idx];
		}

		// move assignment
		vector<T> &operator=(vector<T>&& other) noexcept
		{
			other.Copy(*this);

			return *this;
		}

		// copy assignment
		vector<T> &operator=(const vector<T> &other) noexcept
		{
			Copy(other);

			return *this;
		}

		//
		void Copy(vector<T> &other)
		{
			Reserve(other.DataSize());

			for (size_t i = 0; i < other.Size(); i++)
				m_data[i] = other[i];

			m_lastUsedIdx = other.Size();
		}

		//
		void Sort(int(*cmp)(T *, T *))
		{
			nsl::sort(m_data, m_lastUsedIdx, cmp);
		}

		//
		size_t Size()
		{
			return m_lastUsedIdx;
		}

		// 
		size_t Capacity()
		{
			return (m_allocSize / sizeof(T));
		}

		// Maximum number of entries
		size_t MaxSize()
		{
			return SIZE_MAX / sizeof(T);
		}

		//
		T *Data()
		{
			return m_data;
		}

		//
		size_t DataSize()
		{
			return m_allocSize;
		}

		size_t Push(T data)
		{
			m_lastUsedIdx++;

			// Will we hit invalid memory?
			if (m_lastUsedIdx > Capacity())
				Increment(1);

			m_data[m_lastUsedIdx - 1] = data;

			return m_lastUsedIdx;
		}

		size_t Push(T *data, size_t size)
		{
			// Will we hit invalid memory?
			if ((m_lastUsedIdx + size) > Capacity())
				Increment(size);

			m_lastUsedIdx += size;

			for (size_t i = 0; i < size; i++)
				m_data[(m_lastUsedIdx - size) + i] = data[i];

			return m_lastUsedIdx;
		}

		size_t Pop(size_t num = 1)
		{
			// No...
			if (num == 0)
				return m_lastUsedIdx;

			// We have to decrement before we adjust the size
			Decrement(num);

			// Whatever is pop'd will be overwritten
			// So we only need to shift the index
			if (num >= m_lastUsedIdx)
				m_lastUsedIdx = 0;
			else
				m_lastUsedIdx -= num;

			return m_lastUsedIdx;
		}

		void Insert(T entry, size_t atIdx)
		{
			m_lastUsedIdx++;

			// Will we hit invalid memory?
			if (m_lastUsedIdx > Capacity())
				Increment(1);

			for (size_t i = m_lastUsedIdx; i > atIdx; --i)
				m_data[i] = m_data[i - 1];

			m_data[atIdx] = entry;
		}

		void Remove(size_t idx)
		{
			for (size_t i = idx; i < m_lastUsedIdx; i++)
				m_data[i] = m_data[i + 1];

			m_lastUsedIdx--;
		}

		void Erase(size_t startIdx, size_t endIdx)
		{
			// Sanity check
			if (startIdx > m_lastUsedIdx || startIdx > endIdx)
				return;

			// If we are just erasing the end, no need to get complex
			if (endIdx >= m_lastUsedIdx)
			{
				Pop(endIdx - startIdx);
				return;
			}

			size_t shiftIdx = endIdx + 1;
			size_t shiftSize = shiftIdx - startIdx;

			for (size_t i = 0; i < shiftSize; i++)
				m_data[startIdx + i] = m_data[shiftIdx + i];

			// Remove the stuff at the end
			m_lastUsedIdx -= shiftSize;

			// Realloc if need be
			Decrement(shiftSize);
		}

		void Reserve(size_t reserveSize)
		{
			if (reserveSize > 0)
			{
				size_t currentSize = Size();

				T* tmp = new T[reserveSize * sizeof(T)];

				if (m_data)
				{
					// Sanitize for making the buffer smaller
					if (reserveSize < currentSize)
						currentSize = reserveSize;

					// Copy the old data if any exists
					for (size_t i = 0; i < currentSize; i++)
						tmp[i] = m_data[i];

					// Do away with it!
					delete m_data;
				}

				m_data = tmp;

				m_allocSize = reserveSize * sizeof(T);
			}
			else
			{
				m_allocSize = 0;

				if (m_data)
					delete m_data;

				m_data = nullptr;
			}
		}

		void Clear()
		{
			m_allocSize = 0;
			m_lastUsedIdx = 0;

			// I was going to use zero here
			// but if they're clearing, they're probably intending on using it again
			// want to keep something around just in case.
			Reserve(SizeIncrement);
		}

	private:
		size_t GetNearestTop(size_t index)
		{
			size_t utilizedIncrements = index / SizeIncrement;
			size_t remainder = index % SizeIncrement;
			size_t top = utilizedIncrements * SizeIncrement;

			if (remainder > 0)
				top += SizeIncrement;

			return top;
		}

		void Increment(size_t num)
		{
			size_t req = Size() + num;
			size_t top = GetNearestTop(req);

			// If it matches exactly, we should add another because why not
			if (top == req)
				top += SizeIncrement;

			// If we still aren't larger than our current capacity
			// We don't need to do anything
			if (top <= Capacity())
				return;

			// Reserve the required number
			Reserve(top);
		}

		void Decrement(size_t num)
		{
			// We should never deallocate for less than this
			if (num < SizeIncrement)
				return;

			// Junk it all
			if (Size() <= num)
			{
				Reserve(SizeIncrement);
				return;
			}

			size_t req = Size() - num;
			size_t top = GetNearestTop(req);

			// We tried.jpg
			if (top == Capacity())
				return;

			Reserve(top);
		}

		//
		T *m_data;

		//
		size_t m_lastUsedIdx;
		size_t m_allocSize;
	};

}