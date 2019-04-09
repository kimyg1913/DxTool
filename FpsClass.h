#pragma once

#include "Base.h"

class FpsClass
{
public:
	FpsClass();
	~FpsClass();

	void Initialize();
	void Update();
	int GetFps();

private:
	int m_fps, m_count;
	unsigned long m_startTime;
};

