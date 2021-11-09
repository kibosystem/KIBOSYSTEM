#pragma once


// TextCtrl

class TextCtrl : public CStatic
{
	DECLARE_DYNAMIC(TextCtrl)

public:
	TextCtrl();
	virtual ~TextCtrl();
	void SetText(LPWSTR lpsz,...);
protected:
	DECLARE_MESSAGE_MAP()
};


