#include "stdafx.h"
#include "FpsClass.h"


FpsClass::FpsClass()
{
}


FpsClass::~FpsClass()
{
}

void FpsClass::Initialize()
{
	m_fps = 0; 
	m_count = 0; 
	m_startTime = timeGetTime(); 
	return;
}

void FpsClass::Update()
{
	m_count++;


	DWORD currentTime = timeGetTime();

	if (currentTime >= m_startTime + 1000)
	{
		m_fps = m_count;
		m_count = 0;

		m_startTime = currentTime;
	}
}

int FpsClass::GetFps()
{

	return m_fps;

}
