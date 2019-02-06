#pragma once

#include "Base.h"

using namespace MyStruct;

template <typename T = float>
struct DATA_2D
{
public:
	DATA_2D(int w = 0, int h = 0) : m_width(w), m_height(h), m_size(w*h), m_data(m_size)
	{};

	void resize(int w, int h)
	{
		m_width = w;
		m_height = h;
		m_size = w * h;
		m_data.resize(w*h);
	}

	int width() const { return m_width; }
	int height() const { return m_height; }
	int size() const { return m_size; }

	T & operator() (int y, int x) { return m_data[y*m_width + x]; };
	const T & operator () (int y, int x) const { return m_data[y*m_width + x]; }
	T & operator() (int i) { return m_data[i]; };
	const T & operator () (int i) const { return m_data[i]; }
private:
	int m_width;
	int m_height;
	int m_size;
	vector<T> m_data;
};

class ErosionClass
{
	DECLAER_SINGLE(ErosionClass);
public:

	bool Update(float dt, LPDIRECT3DVERTEXBUFFER9 pVB);
	bool Init(int width, int height);
	void AddRainDrop(D3DXVECTOR3 pos);
	void SetRainRadius(float rad);
	void SetRainAmount(float amount);

	float GetRainRadius() { return m_fRainRadius; }
private:
	bool MakeWater(float dt);
	bool SimulateFlux(float dt, LPDIRECT3DVERTEXBUFFER9 pVB);
	bool SimulateErosion(float dt, LPDIRECT3DVERTEXBUFFER9 pVB);
	bool SimulateSedimentTransportation(float dt);
	bool SimulateEvaporation(float dt, LPDIRECT3DVERTEXBUFFER9 pVB);
	void SmoothTerrain(float dt, LPDIRECT3DVERTEXBUFFER9 pVB);
	void ComputeSurfaceNormals(LPDIRECT3DVERTEXBUFFER9 pVB);
	void SunErosion(float dt, LPDIRECT3DVERTEXBUFFER9 pVB);

	float GetRFlux(int y, int x);
	float GetLFlux(int y, int x);
	float GetTFlux(int y, int x);
	float GetDFlux(int y, int x);
	float GetWater(int y, int x);
	float GetTerrain(int y, int x);
	//float GetSediment(int y, int x);

private:
	DATA_2D<> m_Water;
	DATA_2D<> m_Terrain;
	DATA_2D<> m_Sediment;
	DATA_2D<> m_tmpSediment;
	DATA_2D<> m_VelosityU;
	DATA_2D<> m_VelosityV;
	DATA_2D<> m_FluxR;
	DATA_2D<> m_FluxL;
	DATA_2D<> m_FluxT;
	DATA_2D<> m_FluxD;
	DATA_2D<D3DXVECTOR3> m_Normal;

	bool m_IsRain;
	bool m_HasWater;

	float m_fRainRadius;
	float m_fRainAmount;

	float m_Gravity;

	int m_Width;
	int m_Height;
};

