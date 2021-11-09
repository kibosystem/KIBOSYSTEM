#pragma once
#include <windows.h>

class KiboCriticalSection
{
private:
	CRITICAL_SECTION m_CriticalSection;
public:
	KiboCriticalSection(void);
	~KiboCriticalSection(void);

	void Enter();
	void Leave();
};
