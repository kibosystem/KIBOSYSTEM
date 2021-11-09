
// MainFrm.h : CMainFrame �N���X�̃C���^�[�t�F�C�X
//

#pragma once
#include "FileView.h"
#include "ClassView.h"
#include "OutputWnd.h"
#include "PropertiesWnd.h"
#include "ToolBarPlus.h"


#define IDM_USER	 10000 //�R�}���h���j���[
#define IDM_USER_MAX 10100 //�ő�R�}���h�~���[�ԍ�

class CMainFrame : public KIBOFrame
{
	
protected: // �V���A��������̂ݍ쐬���܂��B
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:
//	CSortStringArray m_aryCommandPath;

// ����
public:
	void OnCommandFile(UINT unID);//�R�}���h�t�@�C�����s
	void OnHome();
	void OnRight();
	void OnLeft();
	void OnUp();
	void OnDown();
	void OnPageUp();
	void OnPageDown();
	void OnPlus();
	void OnMinus();
	void OnFileOpen();
	void OnA();
	void OnB();
	void OnC();
	void OnD();
	void OnE();
	void OnF();
	void OnG();
	void OnH();
	void OnI();
	void OnJ();
	void OnK();
	void OnL();
	void OnM();
	void OnN();
	void OnO();
	void OnP();
	void OnQ();
	void OnR();
	void OnS();
	void OnT();
	void OnU();
	void OnV();
	void OnW();
	void OnX();
	void OnY();
	void OnZ();
	void OnFunctionKey(int nVK);
	void OnF1();
	void OnF2();
	void OnF3();
	void OnF4();
	void OnF5();
	void OnF6();
	void OnF7();
	void OnF8();
	void OnF9();
	void OnF10();
	void OnF11();
	void OnF12();

	void On800();
	void On1024();
	void On2048();
	void On2592();	
	void OnStreatch();//�g��k��

	// �I�[�o�[���C�h
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ����
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
	virtual void SetStatusBarText(LPCTSTR lpszText);


public:
	CMFCStatusBar     m_wndStatusBar;
	CPropertiesWnd    m_wndProperties;

protected:  // �R���g���[�� �o�[�p�����o�[
	CMFCMenuBar       m_wndMenuBar;
	ToolBarPlus       m_wndToolBar;
	CFileView         m_wndFileView;
	COutputWnd        m_wndOutput;

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnUser1(WPARAM wParam, LPARAM lParam);//���[�U�[�R�}���h1
	afx_msg LRESULT OnUser2(WPARAM wParam, LPARAM lParam);//���[�U�[�R�}���h2
	afx_msg LRESULT OnUser3(WPARAM wParam, LPARAM lParam);//���[�U�[�R�}���h3
	afx_msg LRESULT OnUser4(WPARAM wParam, LPARAM lParam);//���[�U�[�R�}���h4
	afx_msg LRESULT OnUser5(WPARAM wParam, LPARAM lParam);//���[�U�[�R�}���h5
	afx_msg LRESULT OnUser6(WPARAM wParam, LPARAM lParam);//���[�U�[�R�}���h6
	afx_msg LRESULT OnUser7(WPARAM wParam, LPARAM lParam);//���[�U�[�R�}���h7
	afx_msg LRESULT OnUser8(WPARAM wParam, LPARAM lParam);//���[�U�[�R�}���h8
	afx_msg LRESULT OnUser9(WPARAM wParam, LPARAM lParam);//���[�U�[�R�}���h9
	afx_msg LRESULT OnUserA(WPARAM wParam, LPARAM lParam);//���[�U�[�R�}���hA
	afx_msg LRESULT OnUserB(WPARAM wParam, LPARAM lParam);//���[�U�[�R�}���hB
	afx_msg LRESULT OnUserC(WPARAM wParam, LPARAM lParam);//���[�U�[�R�}���hC
	afx_msg LRESULT OnUserD(WPARAM wParam, LPARAM lParam);//���[�U�[�R�}���hD

	afx_msg LRESULT OnBinaryDialog(WPARAM wParam, LPARAM lParam);  //�o�C�i���_�C�A���O
	afx_msg LRESULT OnInit(WPARAM wParam, LPARAM lParam); //����������
	afx_msg void OnCameraFile();//�J�����摜�ۑ�
	afx_msg void OnUpdateEnable(CCmdUI* pCmdUI);
public:
	virtual BOOL OnShowPopupMenu(CMFCPopupMenu* pMenuPopup);	
	LRESULT OnCalibration(WPARAM wParam, LPARAM lParam);//�L�����u���[�V����

};

extern CMainFrame* GetMainFrame();
