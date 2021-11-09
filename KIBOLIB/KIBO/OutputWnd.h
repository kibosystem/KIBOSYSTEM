#pragma once


// OutputWnd

class OutputWnd : public CDockablePane
{
	DECLARE_DYNAMIC(OutputWnd)

public:
	OutputWnd();
	virtual ~OutputWnd();

	virtual void Clear() {}
	virtual void OutputText(int nNo, CString &str) {}
	virtual void OutputText(CString &str) {}
protected:
	DECLARE_MESSAGE_MAP()
};


extern OutputWnd *GetOutputWnd();