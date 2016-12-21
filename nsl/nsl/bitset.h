#pragma once

#include "types.h"

namespace nsl
{
	template<size_t N>
	struct bitset
	{
		// default constructor
		bitset() noexcept {}

		// copy constructor
		bitset(bitset<N> &other) = default;

		// virtual for dynamic_cast
		virtual ~bitset() noexcept {}

		struct bit
		{
			friend bitset;

			bit &operator=(bool value)
			{
				BIT_CLEAR(*m_value, m_position);

				if (value)
					BIT_SET(*m_value, m_position);

				return *this;
			}

			bit &operator=(int value)
			{
				if (value > 1)
					value = 1;

				// It's "not zero" so here I go.
				if (value < 0)
					value = 1;

				BIT_CLEAR(*m_value, m_position);

				if (value)
					BIT_SET(*m_value, m_position);

				return *this;
			}

			bool operator==(int value)
			{
				if (value > 1)
					value = 1;

				if (value < 0)
					value = 0;

				return (BIT_CHECK(*m_value, m_position) != 0 && value != 0);
			}

			explicit operator int() const
			{
				return (BIT_CHECK(*m_value, m_position) > 0);
			}

			uint8_t get()
			{
				return (BIT_CHECK(*m_value, m_position) > 0);
			}
		protected:
			bit(uint8_t *value, uint8_t position) : m_value(value), m_position(position) {}

		private:
			uint8_t *m_value;
			uint8_t m_position;
		};

		// index getter
		bit operator[](size_t idx) noexcept
		{
			return bit(&m_data[(idx == 0) ? 0 : (idx / 8)], (idx % 8));
		}

		// index getter (const)
		const bit operator[](size_t idx) const noexcept
		{
			return bit(&m_data[(idx == 0) ? 0 : (idx / 8)], (idx % 8));
		}

		void set(size_t idx, bool value)
		{
			size_t bi = (idx == 0) ? 0 : (idx / 8);
			uint8_t *byte = &m_data[bi];
			uint8_t pos = (idx % 8);

			if (BIT_CHECK(*byte, pos) != ((value == true) ? 1 : 0))
				BIT_FLIP(*byte, pos);
		}

		void set(size_t idx, int value)
		{
			if (value > 1)
				value = 1;

			// It's "not zero" so here I go.
			if (value < 0)
				value = 1;

			return set(idx, (value == 1 ? true : false));
		}

		uint8_t get(size_t idx)
		{
			size_t bi = (idx == 0) ? 0 : (idx / 8);
			uint8_t *byte = &m_data[bi];
			uint8_t pos = (idx % 8);

			return (BIT_CHECK(*byte, pos) > 0);
		}

	private:

		void GetBitInfo(size_t idx, size_t *byteNumber, size_t *byteOffset)
		{
			if (byteNumber)
				*byteNumber = (idx == 0) ? 0 : (idx / 8);

			if (byteOffset)
				*byteOffset = (idx % 8);
		}

		// We need to reserve room for the last
		// entry, in case they set an uneven number
		// of bits. I could restrict to divisible by 8
		// but why bother when the solution is simple?
		uint8_t m_data[(N / 8) + 1];
	};
}