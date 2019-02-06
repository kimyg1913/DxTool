#include "stdafx.h"
#include <math.h>
#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>
#include "PerlinClass.h"

#define PERLIN_PI 3.141592653589793240

DEFINITION_SINGLE(PerlinClass)

PerlinClass::PerlinClass()
{
	p = new int[512];
	for (int x = 0; x < 512; x++) {
		p[x] = m_permutation[x % 256];
	}
}


PerlinClass::~PerlinClass()
{
	delete[] p;
}

void PerlinClass::Setup(double amplitude, double persistence, double frequency, int octaves)
{
	m_amplitude = amplitude;
	m_persistence = persistence;
	m_frequency = frequency;
	m_octaves = octaves;
}

double PerlinClass::ComputeNoise(double x)
{
	return 0.0;
}

double PerlinClass::ComputeNoise2D(double x, double y)
{
	double total = 0;
	double frequency = m_frequency;
	double amplitude = m_amplitude;
	double maxValue = 0;

	for (int i = 0; i < m_octaves; ++i)
	{
		total += InterpolatedNoise2D(x * frequency, y * frequency) * amplitude;
		maxValue += amplitude;

		frequency *= 2.0;
		amplitude *= 0.5;
	}
	//total /= maxValue;
	return total;
}

double PerlinClass::ComputeNoise3D(double x, double y, double z)
{
	double total = 0;
	double frequency = m_persistence;
	double amplitude = m_amplitude;
	double maxValue = 0;			// Used for normalizing result to 0.0 - 1.0

	for (int i = 0; i < m_octaves; i++) 
	{
		total += InterpolatedNoise3D(x * frequency, y * frequency, z * frequency) * amplitude;
		maxValue += amplitude;

		frequency *= 2.0;
		amplitude *= 0.5;
	}

	return total;// / maxValue;
}

double PerlinClass::LinearInterpolate(double a, double b, double x)
{
	return a * (1-x) + b * x;
}

double PerlinClass::CosineInterpolate(double a, double b, double x)
{
	double dX = (1-cos(x * PERLIN_PI)) * 0.5f;

	return a * (1 - dX) + b * dX;
}

double PerlinClass::CubicInterpolate(double v0, double v1, double v2, double v3, double x)
{
	double p = (v3 - v2) - (v0 - v1);
	double x2 = x * x;
	return v1 + (v2 - v0) * x + (v0 - v1 - p) * x2 + p * x2 * x;
}

double PerlinClass::Noise(int n)
{
	n = (n << 13) ^ n;
	return 1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0;
}

double PerlinClass::Noise2D(int x, int y)
{
	int n = x + y * 57;
	n = (n << 13) ^ n;

	return (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

double PerlinClass::SmoothedNoise(int x)
{
	return Noise(x) / 2 + Noise(x - 1) / 4 + Noise(x + 1) / 4;
}

double PerlinClass::SmoothedNoise2D(int x, int y)
{
	double corners = (Noise2D(x - 1, y - 1) + Noise2D(x + 1, y - 1) + Noise2D(x - 1, y + 1) + Noise2D(x + 1, y + 1)) / 16;
	double sides = (Noise2D(x - 1, y) + Noise2D(x + 1, y) + Noise2D(x, y - 1) + Noise2D(x, y + 1)) / 8;
	double center = Noise2D(x, y) / 4;

	return corners + sides + center;
}


double PerlinClass::InterpolatedNoise(double x)
{
	return 0.0;
}

double PerlinClass::InterpolatedNoise2D(double x, double y)
{
	int intX = (int)floor(x) & 255;
	int intY = (int)floor(y) & 255;
	int intXRight = (intX + 1) & 255;
	int intYUp = (intY + 1) & 255;
	
	double fractionX = x - (int)floor(x);
	double fractionY = y - (int)floor(y);

	double fractionXPlus = fractionX - 1;
	double fractionYPlus = fractionY - 1;

	double u = Quintic(fractionX);
	double v = Quintic(fractionY);

	double v1 = Grad2D(p[p[intX]+intY], fractionX, fractionY);
	double v2 = Grad2D(p[p[intXRight] + intY], fractionXPlus, fractionY);
	double v3 = Grad2D(p[p[intX] + intYUp], fractionX, fractionYPlus);
	double v4 = Grad2D(p[p[intXRight] + intYUp], fractionXPlus, fractionYPlus);

	double i1 = CosineInterpolate(v1, v2, u);
	double i2 = CosineInterpolate(v3, v4, u);

	return CosineInterpolate(i1, i2, v);
}

double PerlinClass::InterpolatedNoise3D(double x, double y, double z)
{
	int intX = (int)floor(x) & 255;
	int intY = (int)floor(y) & 255;
	int intZ = (int)floor(z) & 255;

	int intXRight = (intX + 1) & 255;
	int intYUp = (intY + 1) & 255;
	int intZFront = (intZ + 1) & 255;

	double fractionX = x - (int)floor(x);
	double fractionY = y - (int)floor(y);
	double fractionZ = z - (int)floor(z);
	double fractionXPlus = fractionX - 1;
	double fractionYPlus = fractionY - 1;
	double fractionZPlus = fractionZ - 1;


	double u = Quintic(fractionX);
	double v = Quintic(fractionY);
	double w = Quintic(fractionZ);
	
	double v1 = Grad(p[p[p[intX] + intY] + intZ], fractionX, fractionY, fractionZ);
	double v2 = Grad(p[p[p[intXRight] + intY]+intZ], fractionXPlus, fractionY, fractionZ);
	double v3 = Grad(p[p[p[intX] + intYUp]+intZ], fractionX, fractionYPlus, fractionZ);
	double v4 = Grad(p[p[p[intXRight]+intYUp] + intZ], fractionXPlus, fractionYPlus, fractionZ);

	double v5 = Grad(p[p[p[intX] + intY] + intZFront], fractionX, fractionY, fractionZPlus);
	double v6 = Grad(p[p[p[intXRight] + intY] + intZFront], fractionXPlus, fractionY, fractionZPlus);
	double v7 = Grad(p[p[p[intX] + intYUp] + intZFront], fractionX, fractionYPlus, fractionZPlus);
	double v8 = Grad(p[p[p[intXRight] + intYUp] + intZFront], fractionXPlus, fractionYPlus, fractionZPlus);

	double i1 = CosineInterpolate(v1, v2, u);
	double i2 = CosineInterpolate(v3, v4, u);

	double i3 = CosineInterpolate(v5, v6, u);
	double i4 = CosineInterpolate(v7, v8, u);

	double l1 = CosineInterpolate(i1, i2, v);
	double l2 = CosineInterpolate(i3, i4, v);

	return CosineInterpolate(l1, l2, w);
}

double PerlinClass::Quintic(const double t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

double PerlinClass::lerp(double a, double b, double x)
{
	return a + x * (b - a);
}

double PerlinClass::Grad2D(int hash, double x, double y)
{
	switch (hash & 0xF)
	{
	case 0x0: return  x + y;
	case 0x1: return -x + y;
	case 0x2: return  x - y;
	case 0x3: return -x - y;
	case 0x4: return  x + y;
	case 0x5: return -x + y;
	case 0x6: return  x - y;
	case 0x7: return -x - y;
	case 0x8: return  x + y;
	case 0x9: return -x + y;
	case 0xA: return  x - y;
	case 0xB: return -x - y;
	case 0xC: return  x + y;
	case 0xD: return -x + y;
	case 0xE: return  x - y;
	case 0xF: return -x - y;
	default: return 0; // never happens
	}
}

double PerlinClass::Grad(int hash, double x, double y, double z)
{
	switch (hash & 0xF)
	{
	case 0x0: return  x + y;
	case 0x1: return -x + y;
	case 0x2: return  x - y;
	case 0x3: return -x - y;
	case 0x4: return  x + z;
	case 0x5: return -x + z;
	case 0x6: return  x - z;
	case 0x7: return -x - z;
	case 0x8: return  y + z;
	case 0x9: return -y + z;
	case 0xA: return  y - z;
	case 0xB: return -y - z;
	case 0xC: return  y + x;
	case 0xD: return -y + z;
	case 0xE: return  y - x;
	case 0xF: return -y - z;
	default: return 0; // never happens
	}
}

double PerlinClass::gradBit(int hash, double x, double y, double z)
{
	int h = hash & 15;
	// Convert lower 4 bits of hash into 12 gradient directions
	double u = h < 8 ? x : y,
		v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

double PerlinClass::fade(int x)
{
	return x * x * x * (x * (x * 6 - 15) + 10); // 6t^5 - 15t^4 + 10t^3
}
