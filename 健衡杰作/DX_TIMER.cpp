#include"DX_TIMER.h"

TIMER::
TIMER(void)
{
	_TimeInterval = (clock_t)0;
	_EndTime = (clock_t)0;
	return;
}
TIMER::
TIMER(clock_t TimeInterval)
{
	_TimeInterval = (TimeInterval > (clock_t)0) ? TimeInterval : (clock_t)0;
	_EndTime = clock() + TimeInterval;
	return;
}
TIMER::
~TIMER(void)
{
	return;
}
void TIMER::
setTimeInterval(clock_t newTimeInterval)
{
	_TimeInterval = (newTimeInterval > (clock_t)0) ? newTimeInterval : _TimeInterval;
	return;
}
bool TIMER::
isTimeCountEnd(void)
{
	return (clock() >= _EndTime) ? true : false;
}
void TIMER::
ReTiming(void)
{
	_EndTime = clock() + _TimeInterval;
	return;
}
