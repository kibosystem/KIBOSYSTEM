#pragma once

#include "PointDbl.h"

#define PAI			(3.1415926535897932384626433832795)

//���b�Z�[�W�{�b�N�X�\���@Type�w��
extern int UIMessageBox(UINT nType,LPWSTR lpszText,...);

//���b�Z�[�W�{�b�N�X�\�� IDOK��p
extern int UIMessageBox(LPWSTR lpszText,...);

//�G���[���b�Z�[�W��p���b�Z�[�W�{�b�N�X
extern void ErrorMsg(LPWSTR lpszText, ...);


//////// ������ �֘A ///////

extern int SplitToken(TCHAR cSep,CStringArray &sAry,CString& sText);//�w���؂蕶���ŕ�����z��쐬
extern int SplitToken(CString strSep,CStringArray &sAry,CString sText);//�w���؂蕶���ŕ�����z��쐬
extern CString GetStringInt(int n); //�w�萔�l�𕶎���ɕϊ����܂�
extern CString Name(LPWSTR lpsz,int n);//�A�Ԗ��̎擾

//////// File �֘A ///////

extern CString SelectBitmapFile(BOOL isOpen,LPCWSTR lpstrInitialDir);//�r�b�g�}�b�v�t�@�C���I��


//////// Window �֘A ///////

//�w��l��菬�����l�̃����_���Ȑ��l��Ԃ�
extern int GetRandom(int nMax);

//�V���b�g�_�E�������擾
extern void ShutDownNameSet();

extern CString GetFileName(CString &strPath); //�t�@�C�����̎擾
extern CString GetFileTitle(CString &strPath); //�t�@�C�����̎擾
extern int GetFileSize(CString &strPath); //�t�@�C���T�C�Y�擾
extern CString GetLeft(CString& str,LPWSTR lpszSplit);//��؂蕶���ȑO�̕�������擾���܂��B
extern CString GetRight(CString& str,LPWSTR lpszSplit);//��؂蕶���ȍ~�̕�������擾���܂��B

//��_�Ԃ̋������v�Z
extern double GetLength(CPoint &pt1,CPoint &pt2);
extern double GetLength(POINT pt1,POINT pt2);
extern double GetLength(double dbX1,double dbY1,double dbX2,double dbY2);

extern double GetAngle(CSize &vec1,CSize &vec2);//�x�N�g���p�x�擾
extern double GetAngle(PointDbl &vec1,PointDbl &vec2);
extern double GetCos(CSize &vec1,CSize &vec2);//�R�T�C���擾
extern double GetSin(CSize &vec1,CSize &vec2);//�T�C���擾
extern CTime GetLastWriteTime(const CString &strPath);

//�w��t�@�C�����s
extern int Exec(int nShow,LPWSTR lpszExec,LPWSTR lpszFile);

//.ini�t�@�C�����當����擾
extern CString GetPrivateProfileText(LPCWSTR lpszSection, LPCWSTR lpszKey, LPCWSTR lpszPath);
//.ini�t�@�C�����班���l�擾
extern double GetPrivateProfileDbl(LPCWSTR lpszSection,LPCWSTR lpszKey,double dbDefault,LPCWSTR lpszPath);
//.ini�t�@�C���ɐ��l��������
extern void WritePrivateProfileInt(LPCWSTR lpszSection,LPCWSTR lpszKey,int nVal,LPCWSTR lpszPath);
//.ini�t�@�C���Ɏ�����������
extern void WritePrivateProfileDbl(LPCWSTR lpszSection,LPCWSTR lpszKey,LPCTSTR lpszFormat,double dbVal,LPCWSTR lpszPath);

//�F�v�Z
struct YSH
{
	double m_dbY;
	double m_dbS;
	double m_dbH;
};
extern YSH GetYSH(BYTE byR, BYTE byG, BYTE byB); //RGB -> YSH

extern void KeyAction(WORD VirtualKey, BOOL bKeepPressing);//�L�[����