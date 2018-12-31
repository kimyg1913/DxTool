#pragma once
#include <timeapi.h>


class Time
{
public:
	Time() {
		_prevTime = timeGetTime();
	}

	static Time* getInstance()
	{
		static Time time;
		return &time;
	}


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

