#ifndef _DX_TIMER_H_
#define _DX_TIMER_H_

#include<ctime>

class TIMER
{
public:
	clock_t _EndTime;
	clock_t _TimeInterval;

	TIMER(void);
	TIMER(clock_t TimeInterval);
	virtual ~TIMER(void);
	void setTimeInterval(clock_t newTimeInterval);
	bool isTimeCountEnd(void);
	void ReTiming(void);
};

#endif