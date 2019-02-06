#pragma once


namespace MyOperation
{

	template <typename T>
	const T& Clamp(const T & v, const T & minV, const T & maxV)
	{
		return v < minV ? minV : v > maxV ? maxV : v;
	}

	template <typename T>
	const T& MyMin(const T & a, const T & b)
	{
		return a < b ? a : b;
	}

	template <typename T>
	const T& MyMax(const T& a, const T & b)
	{
		return a > b ? a : b;
	}

	float LinearInterpolate(float a, float b, float x)
	{
		return a * (1 - x) + b * x;
	}
}