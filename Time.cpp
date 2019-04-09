#include "stdafx.h"
#include "Time.h"


Time::Time()
{
	_prevTime = timeGetTime();
}

Time::~Time()
{

}