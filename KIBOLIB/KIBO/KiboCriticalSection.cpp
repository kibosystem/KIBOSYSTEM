#include "KiboCriticalSection.h"

KiboCriticalSection::KiboCriticalSection(void)
{
	::InitializeCriticalSection( &m_CriticalSection );
}

KiboCriticalSection::~KiboCriticalSection(void)
{
	::DeleteCriticalSection( &m_CriticalSection );
}

void KiboCriticalSection::Enter()
{
	::EnterCriticalSection( &m_CriticalSection );
}

void KiboCriticalSection::Leave()
{
	::LeaveCriticalSection( &m_CriticalSection );
}