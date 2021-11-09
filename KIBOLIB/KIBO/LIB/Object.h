#pragma once
#include "VisualObject.h"
#include "typedef.h"
#include "ArrayPoint.h"

//���C���`��I�u�W�F�N�g
class LineObject : public VisualObject
{
private:	
	CPoint m_ptE;	  //�I�_
	Color m_color; //�h��Ԃ��F
public:
	LineObject(void);
	virtual ~LineObject(void);

	void SetPointS(int x,int y){SetX(x);SetY(y);}
	void SetPointS(CPoint &pt) {SetX(pt.x);SetY(pt.y);}
	void SetPointE(int x,int y){m_ptE.x=x;m_ptE.y=y;}
	void SetPointE(CPoint &pt) {m_ptE = pt;}
	CPoint GetPointS(){return CPoint(GetX(),GetY());} //�n�_�擾
	CPoint GetPointE(){return m_ptE;} //�I�_�擾
	Color  GetColor(){return m_color;}		//�h��Ԃ��F�擾
	void SetColor(Color cr){m_color = cr;}	//�h��Ԃ��F�擾
	void SetLine(CLine &line){
		SetPointS(line.m_ptS);
		SetPointE(line.m_ptE);
	}
	virtual void Draw(CDC *pDC); //��`�`��	
};

//��`�`��I�u�W�F�N�g
class RectObject : public VisualObject
{
protected:
	bool m_isPaint; //�h��Ԃ��܂����H
	int m_nWidth;	//��
	int m_nHeight;	//����	
	Color m_color; //�h��Ԃ��F
public:
	RectObject(void);
	virtual ~RectObject(void);

	BOOL IsPaint(){return m_isPaint;}		//�h��Ԃ��܂����H
	void SetPaint(bool b){m_isPaint = b;}	//�h��Ԃ��ݒ�
	int  GetWidth(){return m_nWidth;}		//���擾
	int  GetHeight(){return m_nHeight;}		//�����擾
	void SetWidth(int w) {m_nWidth = w;}	//���ݒ�
	void SetHeight(int h){m_nHeight = h;}	//�����ݒ�
	Color GetColor(){return m_color;}			//�h��Ԃ��F�擾
	void SetColor(Color color){m_color = color;}	//�h��Ԃ��F�擾
	void SetRect(CRect &rect); //�\���̈�ݒ�

	virtual void Draw(CDC *pDC); //��`�`��	
};

//�~�`��I�u�W�F�N�g
class CircleObject : public RectObject
{
public:
	CircleObject(void);
	virtual ~CircleObject(void);
	virtual void Draw(CDC *pDC); //��`�`��	
};


//�|���S�����C���`��I�u�W�F�N�g
class PolylineObject : public VisualObject
{
private:
	ArrayPoint m_ptAry; //���E��
	COLORREF m_color;	//�h��Ԃ��F
	bool m_isClosed;	//���܂����H
public:
	PolylineObject();
	virtual ~PolylineObject(void);

	BOOL IsClosed(){return m_isClosed;}			//���܂����H
	void SetClosed(bool b){m_isClosed = b;}		//���邩�ǂ�����ݒ�
	COLORREF GetColor(){return m_color;}		//�h��Ԃ��F�擾
	void SetColor(COLORREF cr){m_color = cr;}	//�h��Ԃ��F�擾
	void SetPoints(ArrayPoint& ptAry){
		m_ptAry.RemoveAll();
		for(int i=0;i<ptAry.GetSize();i++){
			if( ptAry[i].x <= 0 ){
				break;
			}
			m_ptAry.Add( ptAry[i] );
		}
	}
	virtual void Draw(CDC *pDC); //��`�`��	
};

//������`��I�u�W�F�N�g
class StringObject : public VisualObject
{
private:
	CString  m_strText; //�\��������
	COLORREF m_color;	//�����F
	HFONT m_hFont;		//�t�H���g
	CRect m_rect;		//�\���̈�
	UINT m_unFormat;	//�\���̈�t�H�[�}�b�g
public:
	StringObject(void);
	virtual ~StringObject(void);

	CString&  GetText(){return m_strText;} //������擾
	void SetText(CString &s){m_strText=s;} //������ݒ�
	COLORREF GetColor(){return m_color;}		//�h��Ԃ��F�擾
	void SetColor(COLORREF cr){m_color = cr;}	//�h��Ԃ��F�ݒ�
	CFont*  GetFont(){return CFont::FromHandle(m_hFont);}//�t�H���g���擾
	void	SetFont(HFONT h){m_hFont=h;} //�t�H���g�ݒ�
	void SetRect(CString &strH,CString &strV,int nLeft,int nTop,int nRight,int nBottom); //�\���̈�ݒ�	
	void InitRect();//������`��̈揉����
	virtual void Draw(CDC *pDC); //��`�`��	
};
