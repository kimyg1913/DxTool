#pragma once


class PerlinClass
{
public:
	PerlinClass();
	~PerlinClass();

	static PerlinClass * GetInstance() 
	{ 
		static PerlinClass perlin;
		return &perlin;
	}

	void Setup(float amplitude, float persistence, int octaves);
	float ComputeNoise(float x);
	float ComputeNoise2D(float x, float y);

protected:
	float LinearInterpolate(float a, float b, float x);
	float CosineInterpolate(float a, float b, float x);
	float CubicInterpolate(float v0, float v1, float v2, float v3, float x);
	float Noise(int v);
	float Noise2D(int x, int y);
	float SmoothedNoise(int x);
	float SmoothedNoise2D(int x, int y);
	float InterpolatedNoise(float x);
	float InterpolatedNoise2D(float x, float y);
private:
	int m_randomize;
	float m_amplitude;
	float m_persistence;
	int m_octaves;
};

