#pragma once


// Static

class Static : public CStatic
{
	DECLARE_DYNAMIC(Static)

public:
	Static();
	virtual ~Static();

	void SetText(LPCTSTR lpsz,...);
protected:
	DECLARE_MESSAGE_MAP()
};


