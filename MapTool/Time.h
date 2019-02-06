#pragma once
#include <timeapi.h>
#include "Base.h"

class Time
{
	DECLAER_SINGLE(Time);

public:
	float getTick() { return _tick; }
	float getFps() { return _fps; }

	void update()
	{
		DWORD currentTIme = timeGetTime();
		_tick = (currentTIme - _prevTime)/1000.0f;
		_prevTime = currentTIme;

		if (_accTime < 1.0f)
		{
			++_accFps;
		}
		else 
		{
			_fps = _accFps;
			_accTime = 0.0f;
			_accFps = 0;
		}

		_accTime += _tick;
	}

private:
	DWORD _prevTime = 0;
	float _tick = 0.0f;
	float _accTime = 0.0f;
	DWORD _accFps = 0;
	DWORD _fps = 0;

};

