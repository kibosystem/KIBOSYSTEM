#pragma once

#include "color.h"
#include "GlobalMgr.h"
#include "typedef.h"
#include "gdiplus.h"

//�\���p��{�I�u�W�F�N�g
class VisualObject
{
public:
	enum{ //VisualObject Type
		 TYPE_NONE = 0	//�w��Ȃ�
		,TYPE_BITMAP	//Bitmap
		,TYPE_LINE		//��
		,TYPE_RECT		//��`
		,TYPE_CIRCLE	//�~
		,TYPE_STRING	//������
		,TYPE_POLYLINE  //�|���S���̘g��
	};
private: //�\�����W
	bool m_isVisible; //�\�����܂���
	BYTE m_byType; //�I�u�W�F�N�g�^�C�v
	int m_nX;	//x���W
	int m_nY;	//y���W
	CString m_strName; //�I�u�W�F�N�g����肷�邽�߂̖��O
public:
	VisualObject(void);
	virtual ~VisualObject(void);

	bool IsVisible(){return m_isVisible;} //�\�����܂���
	void SetVisible(bool is){m_isVisible=is;} 
	BYTE GetType(){return m_byType;} //�I�u�W�F�N�g�^�C�v
	void SetType(BYTE by){m_byType=by;}
	int GetX(){return m_nX;}	//x���W�擾
	int GetY(){return m_nY;}	//y���W�擾
	void SetX(int x){m_nX = x;} //x���W�ݒ�
	void SetY(int y){m_nY = y;} //y���W�ݒ�
	CString& GetName(){return m_strName;} //�I�u�W�F�N�g���̎擾
	void SetName(CString s){ if(!s.IsEmpty()){m_strName=s;} } //�I�u�W�F�N�g���̐ݒ�

	virtual void Draw(CDC *pDC) = 0; //Visual Object�̕`��
};

//�\���p��{�I�u�W�F�N�g���I�z��
class AryVO : public CArray<VisualObject *>
{
public:
	AryVO(void);
	~AryVO(void);

	int Find(VisualObject *pVO);
};

class ArrayPoint;

//Visual Object�o�^���[��
//�w�肵���^�C�v�������z��ɂ���΂����L���z��Ɉڂ�
//�w�肵���^�C�v���Ȃ���΍쐬����
class VisualObjectMgr
{
private:
	bool m_isVisible;	//�\���E��\���؂�ւ�
	AryVO m_aryValid;	//�L���z��
	AryVO m_aryInvalid;	//�����z��
public:
	VisualObjectMgr();	//�R���X�g���N�^
	~VisualObjectMgr();	//�f�X�g���N�^
	void Draw(CDC *pDC); //Visual Object�̕`��

	void SetVisible(bool b){m_isVisible = b;} //�\���E��\���؂�ւ�
	bool IsVisible(){return m_isVisible;} //�\�����܂����H
	void CreateLineObject(CString strName,const CLine &line,Color color);//���C���ǉ�
	void CreateRectObject(CString strName,const CRect &rect,Color color,BOOL isPaint); //��`�ǉ�
	void CreateCircleObject(CString strName,const CRect &rect,Color color,BOOL isPaint); //�~�ǉ�
	void CreatePolylineObject(CString strName,ArrayPoint &ptAry,COLORREF color,bool isClosed=true); //�|���S���g���ǉ�	
	void CreatePolylineObject(CString strName,const LPPOINT lpPoints,int nCnt,COLORREF color,bool isClosed =true);//�|���S���g���ǉ�
	void CreateStringObject(CString strName,int nX,int nY,CString strText,COLORREF color,LOGFONT &lf); //������ǉ�

	void SetFront(VisualObject *pVO); //�w��VisualObject���őO�ʂɈړ�
	void Remove(VisualObject *pVO); //�w��VisualObject�𖳌���
	void RemoveAll(); //�SVisualObject�𖳌���
	void RemoveByName(const CString &strName); //�w�薼�̂��܂ރI�u�W�F�N�g�𖳌������܂�	
	void RemoveExcept(const CString &strName);//�w��I�u�W�F�N�g�ȊO���폜
	VisualObject* GetVO(CString strName); //�w��Visual Object�擾
private:
	VisualObject* CreateVisualObject(int nType,CString &strName);//Object�̍쐬
};
