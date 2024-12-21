#pragma once

namespace Sgl
{
	class TimeSpan
	{
	private:
		float _milliseconds;
	public:
		TimeSpan(float milliseconds):
			_milliseconds(milliseconds)
		{}

		float Milliseconds() const { return _milliseconds; }
		float Seconds() const { return _milliseconds / 1000; }
		float Microseconds() const { return _milliseconds * 1000; }

		operator unsigned int() const { return _milliseconds; }

		TimeSpan& operator+(TimeSpan other)
		{
			_milliseconds += other._milliseconds;
			return *this;
		}

		TimeSpan& operator-(TimeSpan other)
		{
			_milliseconds -= other._milliseconds;
			return *this;
		}

		TimeSpan& operator*(TimeSpan other)
		{
			_milliseconds *= other._milliseconds;
			return *this;
		}

		TimeSpan& operator/(TimeSpan other)
		{
			_milliseconds /= other._milliseconds;
			return *this;
		}

		bool operator>(TimeSpan other) const
		{
			return _milliseconds > other._milliseconds;
		}

		bool operator<(TimeSpan other) const
		{
			return !operator>(other);
		}

		bool operator>=(TimeSpan other) const
		{
			return _milliseconds >= other._milliseconds;
		}

		bool operator<=(TimeSpan other) const
		{
			return !operator>=(other);
		}

		bool operator==(TimeSpan other) const
		{
			return _milliseconds == other._milliseconds;
		}

		bool operator!=(TimeSpan other) const
		{
			return !operator==(other);
		}
	};
}