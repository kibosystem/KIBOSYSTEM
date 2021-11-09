#pragma once


// ToolBarPlus

class ToolBarPlus : public CMFCToolBar
{
	DECLARE_DYNAMIC(ToolBarPlus)

public:
	ToolBarPlus();
	virtual ~ToolBarPlus();

	BOOL OnToolTipNotify(UINT id, NMHDR * pNMHDR, LRESULT * pResult);
protected:
	DECLARE_MESSAGE_MAP()
};


