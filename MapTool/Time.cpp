#include "stdafx.h"
#include "Time.h"

DEFINITION_SINGLE(Time)

Time::Time()
{
		_prevTime = timeGetTime();
}

Time::~Time()
{

}