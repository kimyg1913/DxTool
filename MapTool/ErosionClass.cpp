#include "stdafx.h"
#include "ErosionClass.h"
#include <algorithm>
#include "MyOperation.h" 
#include "TerrainClass.h"
#include <random>

using namespace MyOperation;
using namespace MyStruct;

DEFINITION_SINGLE(ErosionClass)

ErosionClass::ErosionClass() : m_Water(), m_Terrain(), m_Sediment(), m_tmpSediment(),
m_VelosityU(), m_VelosityV(), m_FluxR(), m_FluxL(), m_FluxT(), m_FluxD(), m_Normal(),
m_IsRain(false), m_HasWater(false), m_Gravity(9.81), m_Width(0), m_Height(0)
{
}

ErosionClass::~ErosionClass()
{
}

bool ErosionClass::Init(int width, int height)
{
	m_Width = width;
	m_Height = height;

	m_HasWater = true;

	DATA_2D<> * pArr[10] = { &m_Water, &m_Terrain,&m_Sediment, &m_tmpSediment, &m_VelosityU, &m_VelosityV, &m_FluxR, &m_FluxL, &m_FluxT, &m_FluxD };

	for (int i = 0; i < 10; ++i)
	{
		*pArr[i] = DATA_2D<>(width, height);
	}

	m_Normal = DATA_2D<D3DXVECTOR3>(m_Width*m_Height);

	MakeWater(0);

	return true;
}

void ErosionClass::AddRainDrop(D3DXVECTOR3 pos)
{

	m_HasWater = true;
	float rad2 = m_fRainRadius * m_fRainRadius;
	float d;
	vector<int> vPickedIndex = GET_SINGLE(TerrainClass)->GetVectorPickedIndex();

	TERRAINVERTEX * pVertices;

	GET_SINGLE(TerrainClass)->GetVertexBuffer()->Lock(0, 0, (void**)&pVertices, 0);

	for (int z = 0; z < m_Height; ++z)
	{
		for (int x = 0; x < m_Width; ++x)
		{
			m_Terrain(z, x) = pVertices[z * m_Width + x].point.y;
		}
	}

	for (int i = 0; i < vPickedIndex.size(); ++i)
	{
		float dX = pos.x - pVertices[vPickedIndex[i]].point.x;
		float dZ = pos.z - pVertices[vPickedIndex[i]].point.z;

		d = sqrtf(dX * dX + dZ * dZ);
		float a = m_fRainAmount * (m_fRainRadius - d) * 1;
		m_Water(vPickedIndex[i]) += a;
	}

	GET_SINGLE(TerrainClass)->GetVertexBuffer()->Unlock();

}

void ErosionClass::SetRainRadius(float rad)
{
	m_fRainRadius = rad;
}

void ErosionClass::SetRainAmount(float amount)
{
	m_fRainAmount = amount;
}

typedef std::mt19937 RANDOM;
RANDOM rnd;

bool ErosionClass::MakeWater(float dt)
{
	std::uniform_int_distribution<int> rndInt(1, m_Width - 2);
	
	m_HasWater = true;
	
	for (int i = 0; i < 100; i++)
	{
		int x = rndInt(rnd);
		int y = rndInt(rnd);
		//int x = 30;
		//int y = 30;
		//        water(y,x) += 1;
		m_Water(y - 1, x - 1)		 += 25;
		m_Water(y - 1, x)			 += 25;
		m_Water(y - 1, x + 1)		 += 25;
		m_Water(y, x - 1)			 += 25;
		m_Water(y, x)				 += 25;
		m_Water(y, x + 1)			 += 25;
		m_Water(y + 1, x - 1)		 += 25;
		m_Water(y + 1, x)			 += 25;
		m_Water(y + 1, x + 1)		 += 25;
	}
	
	//for (int i = 0; i < m_Width * m_Height; ++i)
	//{
	//	m_Water(i) += 1.01;
	//}

	return true;
}

bool ErosionClass::SimulateFlux(float dt, LPDIRECT3DVERTEXBUFFER9 pVB)
{
	float length = 1;
	float A = 20;

	const float dx = 1.0f; //그리드 간격
	const float dy = 1.0f;

	float fluxFactor = dt * A * m_Gravity / length;

	TERRAINVERTEX * pVertices;

	pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (int y = 0; y < m_Height; ++y)
	{
		for (int x = 0; x < m_Width; ++x)
		{

			float differHeight;
			float height0 = pVertices[y * m_Width + x].point.y + m_Water(y, x);
			float newFlux = 0.0f;

			//left outflow
			if (x > 0)
			{
				differHeight = height0 - pVertices[y * m_Width + x-1].point.y + m_Water(y, x - 1);
				newFlux =  m_FluxL(y,x) + fluxFactor * differHeight;
				m_FluxL(y, x) = std::max<float>(0.0f, newFlux); // 물이 들오와야하는경우 0으로 처리
			}
			else
			{
				m_FluxL(y, x) = 0.0f; //범위가 넘어감
			}

			//right outflow
			if (x < m_Width - 1)
			{
				differHeight = height0 - pVertices[y * m_Width + x + 1].point.y + m_Water(y, x + 1);
				newFlux = m_FluxR(y, x) + fluxFactor * differHeight;
				m_FluxR(y, x) = std::max<float>(0.0f, newFlux);
			}
			else
			{
				m_FluxR(y, x) = 0.0f;
			}

			// top outflow
			if (y > 0)
			{
				differHeight = height0 - pVertices[(y-1) * m_Width + x].point.y + m_Water(y - 1, x);
				newFlux = m_FluxT(y,x) + fluxFactor * differHeight;
				m_FluxT(y, x) = std::max<float>(0.0f, newFlux);
			}
			else
			{
				m_FluxT(y, x) = 0.0f; // boundary
			}

			// down outflow
			if (y < m_Height - 1) {
				differHeight = height0 - pVertices[(y + 1) * m_Width + x].point.y + m_Water(y + 1, x);
				newFlux =  m_FluxD(y,x) + fluxFactor * differHeight;
				m_FluxD(y, x) = std::max<float>(0.0f, newFlux);
			}
			else
			{
				m_FluxD(y, x) = 0.0f; // boundary
			}

			float sumFlux = m_FluxL(y, x) + m_FluxR(y, x) + m_FluxT(y, x) + m_FluxD(y, x);
			float K;

			/*if (sumFlux*dt <= 0)
				K = 0;
			else*/
				K = std::min<float>(1.0f, float((m_Water(y, x)*dx*dy) / (sumFlux*dt)));
			
			m_FluxL(y, x) *= K;
			m_FluxR(y, x) *= K;
			m_FluxT(y, x) *= K;
			m_FluxD(y, x) *= K;
		}
	}

	pVB->Unlock();
	
	for (int y = 0; y < m_Height; ++y)
	{
		for (int x = 0; x < m_Width; ++x)
		{
			float inFlow = GetRFlux(y, x - 1) + GetLFlux(y, x + 1) + GetDFlux(y + 1, x) + GetTFlux(y - 1, x);
			float outFlow = GetRFlux(y, x) + GetLFlux(y, x) + GetTFlux(y, x) + GetDFlux(y, x);
			float dV = dt * (inFlow - outFlow);
			float oldWater = m_Water(y, x);
			m_Water(y, x) += dV;		// dV/1 인데 면적이 1 이라 생략
			m_Water(y, x) = std::max<float>(m_Water(y, x), 0.0f);

			//float meanWater = 0.5 * (oldWater + m_Water(y, x));

			if (m_Water(y, x) <= 0.0f)
			{
				m_VelosityU(y, x) = m_VelosityV(y, x) = 0.0f;
			}
			else
			{
				m_VelosityU(y, x) = 0.5 * (GetRFlux(y, x - 1) - GetLFlux(y, x) - GetLFlux(y, x + 1) + GetRFlux(y, x));// meanWater;
				m_VelosityV(y, x) = 0.5 * (GetDFlux(y, x) + GetDFlux(y - 1, x) - GetTFlux(y + 1, x) - GetTFlux(y, x));// meanWater;
			}
		}
	}

	return true;
}

bool ErosionClass::SimulateErosion(float dt, LPDIRECT3DVERTEXBUFFER9 pVB)
{
	const float Kc = 1.5f; // sediment capacity constant
//	const float Ks = 0.001f * 12 * 10; // dissolving constant
//	const float Kd = 0.001f * 12 * 10; // deposition constant

	const float Ks = 1.3; // dissolving constant
	const float Kd = 0.3; // deposition constant

	const float rMin = 1.f;
	const float Kh = 5;

	const float Kdmax = 10.f;

	TERRAINVERTEX * pVertices;

	pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (int y = 0; y < m_Height; ++y)
	{
		for (int x = 0; x < m_Width; ++x)
		{
			float uV = m_VelosityU(y, x);
			float vV = m_VelosityV(y, x);

			int xL = MyOperation::Clamp(x - 1, 0, m_Width - 1);
			int xR = MyOperation::Clamp(x + 1, 0, m_Width - 1);
			int yU = MyOperation::Clamp(y - 1, 0, m_Height - 1);
			int yD = MyOperation::Clamp(y + 1, 0, m_Height - 1);

			D3DXVECTOR3 normal = D3DXVECTOR3(pVertices[y * m_Width + xR].point.y - pVertices[y * m_Width + xL].point.y,
				pVertices[yD * m_Width + x].point.y - pVertices[yU * m_Width + x].point.y,2);

			D3DXVec3Normalize(&normal, &normal);

			D3DXVECTOR3 up(0, 0, 1);

			float cosa = D3DXVec3Dot(&normal, &up);
			float sinAlpha = sin(acos(cosa));
			sinAlpha = std::max<float>(sinAlpha, 0.01f);

			float lMax = m_Water(y, x) <= 0 ? 0 : m_Water(y, x) >= Kdmax ? 1 : 1 - (Kdmax - m_Water(y, x));

			// local sediment capacity of the flow
			float capacity = Kc * sqrtf(uV*uV + vV*vV)*sinAlpha*lMax;

			float delta = (capacity - m_Sediment(y, x));

			m_Terrain(y, x) = std::max<float>(rMin, m_Terrain(y, x) - dt * Kh * Ks * (m_Sediment(y, x) - capacity));

			//delta	이동 할 물의 양
			if (delta > 0.0f)
			{
				float d = Ks * delta * m_Terrain(y,x);
				pVertices[y * m_Width + x].point.y -= d;		//d 
				m_Water(y, x) += d;
				m_Sediment(y, x) += d;
			}
			// deposit onto ground
			else if (delta < 0.0f)
			{
				float d = Kd * delta;
				pVertices[y * m_Width + x].point.y -= d;
				m_Water(y, x) += d;
				m_Sediment(y, x) += d;
			}
		}
	}

	pVB->Unlock();

	return true;
}

bool ErosionClass::SimulateSedimentTransportation(float dt)
{
	for (int y = 0; y < m_Height; ++y)
	{
		for (int x = 0; x < m_Width; ++x)
		{
			// local velocity
			float uV = m_VelosityU(y, x);
			float vV = m_VelosityV(y, x);

			// position where flow comes from
			float fromPosX = float(x) - uV * dt;
			float fromPosY = float(y) - vV * dt;

			// integer coordinates
			int x0 = (int)floor(fromPosX);// < 0 ? 0 : (int)floor(fromPosX);
			int y0 = (int)floor(fromPosY);// < 0 ? 0 : (int)floor(fromPosY);
			int x1 = x0 + 1;
			int y1 = y0 + 1;

			// interpolation factors
			float fX = fromPosX - x0;
			float fY = fromPosY - y0;

			// clamp to grid borders
			x0 = MyOperation::Clamp(x0, 0, int(m_Width - 1));
			x1 = MyOperation::Clamp(x1, 0, int(m_Width - 1));
			y0 = MyOperation::Clamp(y0, 0, int(m_Height - 1));
			y1 = MyOperation::Clamp(y1, 0, int(m_Height - 1));

			float newVal = LinearInterpolate(LinearInterpolate(m_Sediment(y0, x0), m_Sediment(y0, x1), fX), LinearInterpolate(m_Sediment(y1, x0), m_Sediment(y1, x1), fX), fY);
			m_tmpSediment(y, x) = newVal;

		}
	}

	for (int i = 0; i < m_Sediment.size(); ++i)
	{
		m_Sediment(i) = m_tmpSediment(i);
	}

	return true;
}

bool ErosionClass::SimulateEvaporation(float dt, LPDIRECT3DVERTEXBUFFER9 pVB)
{
	const float Ke = 0.015f;//*0.5f;// 0.00011*0.5f; // evaporation constant
	m_HasWater = false;

	float sumWater = 0.0f;

	for (int y = 0; y < m_Height; ++y)
	{
		for (int x = 0; x < m_Width; ++x)
		{
			m_Water(y, x) = std::max<float>(m_Water(y, x)*(1 - Ke * dt), 0.0);

			if (m_Water(y, x) < 0.005f)
			{
				m_Water(y, x) = 0.0f;
				
				continue;
			}

			sumWater += m_Water(y, x);
		}
	}

	if (sumWater > 1)
		m_HasWater = true;
	else
	{
		m_HasWater = false;
	}
	std::cout << sumWater << std::endl;

	return true;
}

void ErosionClass::SmoothTerrain(float dT, LPDIRECT3DVERTEXBUFFER9 pVB)
{
	float maxD = 0.2f;

	TERRAINVERTEX * pVertices;
	pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (int y = 0; y < m_Height; ++y)
	{
		for (int x = 0; x < m_Width; ++x)
		{
			float h = pVertices[y * m_Width + x].point.y;

			int xL = MyOperation::Clamp(x - 1, 0, m_Width - 1);
			int xR = MyOperation::Clamp(x + 1, 0, m_Width - 1);
			int yU = MyOperation::Clamp(y - 1, 0, m_Height - 1);
			int yD = MyOperation::Clamp(y + 1, 0, m_Height - 1);
			
			float hl = pVertices[y * m_Width + xL].point.y;
			float hr = pVertices[y * m_Width + xR].point.y;
			float ht = pVertices[yU * m_Width + x].point.y;
			float hb = pVertices[yD * m_Width + x].point.y;
					  
			float dl = h - hl;
			float dr = h - hr;

			float dt = h - ht;
			float db = h - hb;

			m_tmpSediment(y, x) = h;

			if ((abs(dl) > maxD || abs(dr) > maxD) && dr*dl > 0.0f)
			{
				float avg = (h + hl + hr + ht + hb) / 5;
				m_tmpSediment(y, x) = avg;
			}
			else if ((abs(dt) > maxD || abs(db) > maxD) && dt*db > 0.0f)
			{
				float avg = (h + hl + hr + ht + hb) / 5;
				m_tmpSediment(y, x) = avg;
			}
		}
	}



	for (int y = 0; y < m_Height; ++y)
	{
		for (int x = 0; x < m_Width; ++x)
		{
			pVertices[y * m_Width + x].point.y = m_tmpSediment(y,x);
		}
	}

	pVB->Unlock();
}

void ErosionClass::ComputeSurfaceNormals(LPDIRECT3DVERTEXBUFFER9 pVB)
{
	for (int y = 0; y < m_Height; ++y)
	{
		for (int x = 0; x < m_Width; ++x)
		{
			float r, l, t, b;
			D3DXVECTOR3 N;

			r = GetTerrain(y, x + 1) + GetWater(y, x + 1);
			l = GetTerrain(y, x - 1) + GetWater(y, x - 1);
			t = GetTerrain(y - 1, x) + GetWater(y - 1, x);
			b = GetTerrain(y + 1, x) + GetWater(y + 1, x);

			N = D3DXVECTOR3(l - r, t - b, 2);
			D3DXVec3Normalize(&N, &N);

			m_Normal(y, x) = N;
		}
	}
}

void ErosionClass::SunErosion(float dt, LPDIRECT3DVERTEXBUFFER9 pVB)
{
	float tanB = 0.5f;
	const float Kt = 1.0f;

	TERRAINVERTEX * pVertices;

	pVB->Lock(0, 0, (void**)&pVertices, 0);


	for (int y = 0; y < m_Height - 2; ++y)
	{
		for (int x = 0; x < m_Width - 2; ++x)
		{
			float hX = pVertices[y * m_Width + x + 1].point.y - pVertices[y * m_Width + x].point.y;
			float hY = pVertices[(y + 1) * m_Width + x].point.y - pVertices[y * m_Width + x].point.y;
			
			float h = sqrtf(hX*hX + hY * hY);

			if (tanB < h)
			{
				float KtDt = Kt * dt * (h - tanB);

				float h1 = KtDt * (hX + hY) / h;
				float h2 = KtDt * (hX) / h;
				float h3 = KtDt * (hY) / h;

				pVertices[y * m_Width + x].point.y = h1 + h2 + h3;
			}
		
		}
	}

	pVB->Unlock();
}

float ErosionClass::GetRFlux(int y, int x)
{
	if (x<0 || x>m_FluxR.width() - 1) {
		return 0.0f;
	}
	else {
		return m_FluxR(y, x);
	}
}

float ErosionClass::GetLFlux(int y, int x)
{
	if (x<0 || x>m_FluxL.width() - 1) {
		return 0.0f;
	}
	else {
		return m_FluxL(y, x);
	}
}

float ErosionClass::GetTFlux(int y, int x)
{
	if (y<0 || y>m_FluxL.height() - 1) {
		return 0.0f;
	}
	else {
		return m_FluxT(y, x);
	}
}

float ErosionClass::GetDFlux(int y, int x)
{
	if (y<0 || y>m_FluxD.height() - 1) {
		return 0.0f;
	}
	else {
		return m_FluxD(y, x);
	}
}

float ErosionClass::GetWater(int y, int x)
{
	return m_Water(MyOperation::Clamp(y, 0, m_Height - 1), MyOperation::Clamp(x, 0, m_Width - 1));
}

float ErosionClass::GetTerrain(int y, int x)
{
	return m_Terrain(MyOperation::Clamp(y, 0, m_Height - 1), MyOperation::Clamp(x, 0, m_Width - 1));
}

bool ErosionClass::Update(float dt,LPDIRECT3DVERTEXBUFFER9 pVB)
{
	float dtc = dt * 50;
	
	m_HasWater = true;
	
	if (m_HasWater)
	{
		MakeWater(0);
		SimulateFlux(dtc, pVB);
		SimulateErosion(dtc, pVB);
		SimulateSedimentTransportation(dtc);
		SimulateEvaporation(dtc, pVB);
		SmoothTerrain(dt, pVB);
	}



	return true;
}
