#pragma once

#include "Base.h"

class FpsClass
{
public:
	FpsClass();
	~FpsClass();

	void Initialize();
	void Frame();
	int GetFps();

private:
	int m_fps, m_count;
	unsigned long m_startTime;
};

