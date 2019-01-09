#include "stdafx.h"
#include <math.h> 
#include "PerlinClass.h"

#define PERLIN_PI 3.141592653589793240

PerlinClass::PerlinClass()
{
}


PerlinClass::~PerlinClass()
{
}

void PerlinClass::Setup(float amplitude, float persistence, int octaves)
{
	m_amplitude = amplitude;
	m_persistence = persistence;
	m_octaves = octaves;
}

float PerlinClass::ComputeNoise(float x)
{
	return 0.0;
}

float PerlinClass::ComputeNoise2D(float x, float y)
{
	float total = 0;

	for (int i = 0; i <= m_octaves; ++i)
	{
		float frequency = pow(m_persistence, i);
		float amplitude = pow(m_amplitude, i);

		total = total + InterpolatedNoise2D(x * frequency, y * frequency) * amplitude;
	}

	return total;
}

float PerlinClass::LinearInterpolate(float a, float b, float x)
{
	return a * (1-x) + b * x;
}

float PerlinClass::CosineInterpolate(float a, float b, float x)
{
	float dX = (1-cos(x * PERLIN_PI)) * 0.5f;

	return a * (1 - dX) + b * dX;
}

float PerlinClass::CubicInterpolate(float v0, float v1, float v2, float v3, float x)
{
	float p = (v3 - v2) - (v0 - v1);
	float x2 = x * x;
	return v1 + (v2 - v0) * x + (v0 - v1 - p) * x2 + p * x2 * x;
}

float PerlinClass::Noise(int n)
{
	n = (n << 13) ^ n;
	return 1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0;
}

float PerlinClass::Noise2D(int x, int y)
{
	int n = x + y * 57;
	n = (n << 13) ^ n;

	return (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

float PerlinClass::SmoothedNoise(int x)
{
	return Noise(x) / 2 + Noise(x - 1) / 4 + Noise(x + 1) / 4;
}

float PerlinClass::SmoothedNoise2D(int x, int y)
{
	float corners = (Noise2D(x - 1, y - 1) + Noise2D(x + 1, y - 1) + Noise2D(x - 1, y + 1) + Noise2D(x + 1, y + 1)) / 16;
	float sides = (Noise2D(x - 1, y) + Noise2D(x + 1, y) + Noise2D(x, y - 1) + Noise2D(x, y + 1)) / 8;
	float center = Noise2D(x, y) / 4;

	return corners + sides + center;
}


float PerlinClass::InterpolatedNoise(float x)
{
	return 0.0;
}

float PerlinClass::InterpolatedNoise2D(float x, float y)
{
	int intX = int(x);
	float fractionX = x - intX;
	
	int intY = int(y);
	float fractionY = y - intY;

	float v1 = SmoothedNoise2D(intX, intY);
	float v2 = SmoothedNoise2D(intX + 1, intY);
	float v3 = SmoothedNoise2D(intX, intY + 1);
	float v4 = SmoothedNoise2D(intX + 1, intY + 1);

	float i1 = CosineInterpolate(v1, v2, fractionX);
	float i2 = CosineInterpolate(v3, v4, fractionX);

	return CosineInterpolate(i1, i2, fractionY);
}
