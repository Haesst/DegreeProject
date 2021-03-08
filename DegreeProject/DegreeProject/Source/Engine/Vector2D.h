#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

template <typename T>
struct Vec2D
{
	T x = 0;
	T y = 0;

	Vec2D()
		: x(x), y(y)
	{}

	Vec2D(T x, T y)
		: x(x), y(y)
	{}

	Vec2D(sf::Vector2f sfmlVec)
		: x(sfmlVec.x), y(sfmlVec.y)
	{}

	inline Vec2D<T> operator+(const Vec2D<T>& v2) const
	{
		return Vec2D<T>(x + v2.x, y + v2.y);
	}

	inline Vec2D<T>& operator+=(const Vec2D<T>& v2)
	{
		x += v2.x;
		y += v2.y;
		return *this;
	}

	inline Vec2D<T> operator-(const Vec2D<T>& v2) const
	{
		return Vec2D<T>(x - v2.x, y - v2.y);
	}

	inline friend Vec2D<T>& operator-=(Vec2D<T>& v1, const Vec2D<T>& v2)
	{
		v1.x -= v2.x;
		v1.y -= v2.y;
		return v1;
	}

	inline friend Vec2D<T> operator*(const T& scalar, const Vec2D<T>& other)
	{
		return { other.x * scalar, other.y * scalar };
	}

	inline friend Vec2D<T> operator*(const Vec2D<T>& other, const T& scalar)
	{
		return { other.x * scalar, other.y * scalar };
	}

	inline Vec2D<T>& operator/(const T d) const
	{
		return (d != 0 ? Vec2D<T>(x / d, y / d) : Vec2D<T>());
	}

	inline Vec2D<T>& Zero()
	{
		x = 0;
		y = 0;
		return *this;
	}

	inline Vec2D<T>& One()
	{
		x = 1;
		y = 1;
		return *this;
	}

	inline Vec2D<T>& Right()
	{
		x = 1;
		y = 0;
		return *this;
	}

	inline Vec2D<T>& Left()
	{
		x = -1;
		y = 0;
		return *this;
	}

	inline Vec2D<T>& Up()
	{
		x = 0;
		y = 1;
		return *this;
	}

	inline Vec2D<T>& Down()
	{
		x = 0;
		y = -1;
		return *this;
	}

	inline friend std::ostream& operator<<(std::ostream& stream, const Vec2D<T>& v)
	{
		stream << "[" << v.x << "," << v.y << "]";
		return stream;
	}

	inline Vec2D Normalized()
	{
		float length = sqrtf(x * x + y * y);
		if (length != 0)
		{
			x = x / length;
			y =	y / length;
		}
		return *this;
	}

	inline float GetLength()
	{
		return float length = sqrtf(x * x + y * y);
	}

	inline bool operator==(const Vec2D<T>& v2)
	{
		if (x == v2.x && y == v2.y)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	inline bool operator!=(const Vec2D<T>& v2)
	{
		if (x != v2.x || y != v2.y)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	inline bool NearlyEqual(const Vec2D<T>& v2, float tolerance = 0.0f, float centerOffsetX = 0.0f, float centerOffsetY = 0.0f)
	{
		if (abs(x + centerOffsetX - v2.x) <= tolerance && abs(y + centerOffsetY - v2.y) <= tolerance)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

using Vector2DInt = Vec2D<int>;
using Vector2D = Vec2D<float>;