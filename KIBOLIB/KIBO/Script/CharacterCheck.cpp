#include "../stdafx.h"
#include "../LIB/typedef.h"
#include "../LIB/functions.h"
#include "../LIB/macro.h"
#include "../LIB/PointDbl.h"
#include "../KIBOView.h"
#include "../Commandid.h"
#include "CharRecog.h"

#ifdef USE_CHARRECOG 

//Line Point�̕\��
void CheckResult::ShowLine(LPWSTR szName,CPoint &ptTL,CLine& line,Color color)
{	
	CLine lineDraw(line.m_ptS+ptTL,line.m_ptE+ptTL);
	CString strName;
	strName.Format(L"%s%d%d",szName,ptTL.x,ptTL.y);	
	GetView()->m_mgrVO.CreateLineObject(strName,lineDraw,color.ToCOLORREF());
}

//�ŏ����C���`��
void CheckResult::ShowLine()
{
	ShowLine(L"W",m_ptTL,m_line,Color::Magenta); //�����`��
}
//�ŏ����C���̕\��
void CheckResult::ShowMinLine(SizeDbl &size)
{
	if(m_line.IsValid() && m_line.GetLength() < size.cx ){
		ShowLine();
	}
}
//X�����������ʐݒ� Color::Magenta
void CheckResult::SetLine(CLine &line)
{
	if( m_line.IsValid() ){ //���łɒl���Z�b�g����Ă��鎞
		if( line.GetLength() < m_line.GetLength() ){ //��菬�������C���̎�����
			m_line = line; //�㏑���Z�b�g
		}
	}else{//�l���Z�b�g����Ă��Ȃ���
		//�������ŃZ�b�g
		m_line=line;
	}
}	
void CheckResult::SetLine(CPoint &ptS,CPoint &ptE)
{
	CLine line(ptS,ptE); //���C���쐬
	SetLine(line);
}

bool operator < (const CPoint &ptL,const CPoint &ptR)
{
	if( ptL.x != ptR.x ){
		return (ptL.x < ptR.x);
	}
	return (ptL.y < ptR.y);
}

//���������肷��_�N���X
class LengthPoint
{
public:
	CPoint m_ptMin1;
	CPoint m_ptMin2;
	double m_dbWmin; //�ŏ������Ԃ̋���
	CPoint m_ptMax1;
	CPoint m_ptMax2;
	double m_dbWmax; //�ő勗���Ԃ̋���

public:
	//�f�t�H���g�R���X�g���N�^
	LengthPoint()
	{
		m_dbWmin = 0;
		m_dbWmax = 0;
	}
	~LengthPoint(){}

	//�O�����E���Ɠ������E�����쐬����
	static void GetArrayPointInOut(ArrayPoint &aryPoint,ArrayPoint &aryPointOut,ArrayPoint &aryPointIn)
	{
		ArrayPoint *pAryPoint = &aryPointOut;
		for(int i=0;i<aryPoint.GetSize();i++){
			CPoint &pt = aryPoint[i];
			if( pt.x < 0 ){	
				if( aryPointOut.GetSize() == 0 ){
					pAryPoint = &aryPointOut;
				}else if( aryPointIn.GetSize() == 0 ){
					pAryPoint = &aryPointIn;
				}else{
					pAryPoint = &aryPointIn; //Out,In,In �̏��Ԃɓ_��ǉ�
				}
			}else if(pAryPoint != NULL){
				pAryPoint->Add(pt);
			}
		}
	}
	//�O�����E���Ɠ������E�����쐬����
	static void GetArrayPointInOut(ArrayPoint &aryPoint,ArrayPoint &aryPointOut,ArrayPoint &aryPointIn1,ArrayPoint &aryPointIn2)
	{
		ArrayPoint *pAryPoint = &aryPointOut;
		for(int i=0;i<aryPoint.GetSize();i++){
			CPoint &pt = aryPoint[i];
			if( pt.x < 0 ){	
				if( aryPointOut.GetSize() == 0 ){
					pAryPoint = &aryPointOut;//�O���ւ̏�������
				}else if( aryPointIn1.GetSize() == 0 ){
					pAryPoint = &aryPointIn1; //�������E��1�ւ̏�������
				}else if( aryPointIn2.GetSize() == 0 ){
					pAryPoint = &aryPointIn2; //�������E��2�ւ̏�������
				}else{
					pAryPoint = &aryPointOut; //Out,In1,In2,Out �̏��Ԃɓ_��ǉ�
				}
			}else if(pAryPoint != NULL){
				pAryPoint->Add(pt);
			}
		}
	}
};

//�w��x���W�Œ��_����ŏ��ɂԂ���Y���W�����߂�
static int ToBtm(Label &label,int x) 
{
	int nCx = label.GetWidth();
	int nCy = label.GetHeight();
	BYTE* pBits = label.GetDstBuf(); //���x�����O���ʎ擾
	BYTE *pFind = pBits + x; //�����J�n�A�h���X
	for(int y=0;y<nCy;y++,pFind+=nCx){
		if( *pFind == MAXBYTE){ //���C���ɂԂ�����
			return y; //y���W�擾
		}
	}
	return -1;
}
//�w��x���W�Œꂩ��ŏ��ɂԂ���Y���W�����߂�
static int ToTop(Label &label,int x) 
{
	int nCx = label.GetWidth();
	int nCy = label.GetHeight();
	int nYs = nCy-1;
	BYTE* pBits = label.GetDstBuf(); //���x�����O���ʎ擾
	BYTE *pFind = pBits +nYs*nCx+ x; //�����J�n�A�h���X
	for(int y=nYs;y>=0;y--,pFind-=nCx){
		if( *pFind  == MAXBYTE){ //���C���ɂԂ�����
			return y; //y���W�擾
		}
	}
	return -1;
}

//�ォ�牺�����̐��̑����`�F�b�N
double CharRecog::TopToBtm(ArrayPoint& aryPointSrc,double dbDelete,double dbLenMin)
{
	ArrayPoint aryPoint;//���E���̃R�s�[�쐬
	aryPoint.Copy(aryPointSrc);

	//���E�����߂�
	int nLeft  = aryPoint.GetLeft().x;	//�ł����̓_���擾
	int nRight = aryPoint.GetRight().x;	//�ł��E�̓_���擾

	aryPoint.RangeX(nLeft+dbDelete,nRight-dbDelete); //�w��͈݂͂̂�Point�ɕύX �����̔����������s�\�̈�Ƃ���

	ArrayPoint aryConnect;
	ArrayPoint aryNotConnect;

	CPoint ptTop = aryPoint.GetTop(); //���_���W�擾
	aryPoint.CollectConnectPoint( ptTop ,aryConnect); //���_���W�Ƃ̗א�Point�̂�
	aryPoint.CollectExceptPoint(aryConnect,aryNotConnect); //�א�Point�ȊO�����W	
	aryNotConnect.SelectConnectPoint( (int)dbDelete ); //�w�萔�ڑ�Point�݂̂�I��

	CPoint ptS,ptE;
	double dbLen = aryConnect.GetLength(aryNotConnect,ptS,ptE);
	if( dbLen < dbLenMin){ //��������
		m_ckResult.SetLine(ptS,ptE);
		return dbLen;
	}
	return dbLenMin;
}
//�����������̐��̑����`�F�b�N
double CharRecog::BtmToTop(ArrayPoint& aryPointSrc,double dbDelete,double dbLenMin)
{
	ArrayPoint aryPoint;//���E���̃R�s�[�쐬
	aryPoint.Copy(aryPointSrc);

	//���E�����߂�
	int nLeft  = aryPoint.GetLeft().x;	//�ł����̓_���擾
	int nRight = aryPoint.GetRight().x;	//�ł��E�̓_���擾
	aryPoint.RangeX(nLeft+dbDelete,nRight-dbDelete); //�w��͈݂͂̂�Point�ɕύX �����̔����������s�\�̈�Ƃ���

	ArrayPoint aryConnect;
	ArrayPoint aryNotConnect;

	CPoint ptBtm = aryPoint.GetBottom(); //���_���W�擾
	aryPoint.CollectConnectPoint( ptBtm,aryConnect); //���_���W�Ƃ̗א�Point�̂�
	aryPoint.CollectExceptPoint(aryConnect,aryNotConnect); //�א�Point�ȊO�����W
	aryNotConnect.SelectConnectPoint( (int)dbDelete ); //�w�萔�ڑ�Point�݂̂�I��

	CPoint ptS,ptE;
	double dbLen = aryConnect.GetLength(aryNotConnect,ptS,ptE);
	if( dbLen < dbLenMin ){ //��������
		m_ckResult.SetLine(ptS,ptE);
		return dbLen;
	}
	return dbLenMin;
}

//������E�����̐��̕��`�F�b�N
double CharRecog::LeftToRight(ArrayPoint& aryPointSrc,double dbDelete,double dbLenMin)
{
	ArrayPoint aryPoint;//���E���̃R�s�[�쐬
	aryPoint.Copy(aryPointSrc);

	//�㉺�����߂�
	int nTop = aryPoint.GetTop().y;	//�ł���̓_���擾
	int nBtm = aryPoint.GetBottom().y;	//�ł����̓_���擾

	aryPoint.RangeY(nTop+dbDelete,nBtm-dbDelete); //�w��͈݂͂̂�Point�ɕύX �����̔����������s�\�̈�Ƃ���

	ArrayPoint aryConnect;
	ArrayPoint aryNotConnect;

	CPoint ptLeft = aryPoint.GetLeft(); //���[���W�擾
	aryPoint.CollectConnectPoint( ptLeft ,aryConnect); //���[���W�Ƃ̗א�Point�̂�
	aryPoint.CollectExceptPoint(aryConnect,aryNotConnect); //�א�Point�ȊO�����W
	aryNotConnect.SelectConnectPoint( (int)dbDelete ); //�w�萔�ڑ�Point�݂̂�I��

	CPoint ptS,ptE;
	double dbLen = aryConnect.GetLength(aryNotConnect,ptS,ptE);
	if( dbLen < dbLenMin ){ //��������
		m_ckResult.SetLine(ptS,ptE);
		return dbLen;
	}
	return dbLenMin;	
}

//�E���獶�����̐��̕��`�F�b�N
double CharRecog::RightToLeft(ArrayPoint& aryPointSrc,double dbDelete,double dbLenMin)
{
	ArrayPoint aryPoint;//���E���̃R�s�[�쐬
	aryPoint.Copy(aryPointSrc);

	//�㉺�����߂�
	int nTop = aryPoint.GetTop().y;	//�ł����̓_���擾
	int nBtm = aryPoint.GetBottom().y;	//�ł��E�̓_���擾
	aryPoint.RangeY(nTop+dbDelete,nBtm-dbDelete); //�w��͈݂͂̂�Point�ɕύX �����̔����������s�\�̈�Ƃ���

	ArrayPoint aryConnect;
	ArrayPoint aryNotConnect;

	CPoint ptRight = aryPoint.GetRight(); //���[���W�擾
	aryPoint.CollectConnectPoint( ptRight,aryConnect); //���[���W�Ƃ̗א�Point�̂�
	aryPoint.CollectExceptPoint(aryConnect,aryNotConnect); //�א�Point�ȊO�����W
	aryNotConnect.SelectConnectPoint( (int)dbDelete ); //�w�萔�ڑ�Point�݂̂�I��

	CPoint ptS,ptE;
	double dbLen = aryConnect.GetLength(aryNotConnect,ptS,ptE);
	if( dbLen < dbLenMin ){ //��������
		m_ckResult.SetLine(ptS,ptE);
		return dbLen;
	}
	return dbLenMin;	
}
 //�������獶
double CharRecog::CenterToLeft(ArrayPoint& aryPointSrc,double dbDelete,double dbLenMin)
{
	ArrayPoint aryPoint;//���E���̃R�s�[�쐬
	aryPoint.Copy(aryPointSrc);
	//�����������ɂ���
	CRect rect = aryPoint.GetRect();
	aryPoint.RangeY(0,rect.CenterPoint().x+dbDelete);
	return RightToLeft(aryPoint,dbDelete,dbLenMin);
}
//��������E
double CharRecog::CenterToRight(ArrayPoint& aryPointSrc,double dbDelete,double dbLenMin)
{
	ArrayPoint aryPoint;//���E���̃R�s�[�쐬
	aryPoint.Copy(aryPointSrc);
	//�E���������ɂ���
	CRect rect = aryPoint.GetRect();
	aryPoint.RangeY(rect.CenterPoint().x-dbDelete,rect.right);
	return RightToLeft(aryPoint,dbDelete,dbLenMin);
}

//�������E���ƊO�����E���Ԃ̋�������
SizeDbl CharRecog::CheckInOut(TCHAR tch,CPoint &pt,ArrayPoint& aryPoint)
{
	ArrayPoint aryPointOut;	//�O�����E��
	ArrayPoint aryPointIn;	//�������E��

	LengthPoint lenPt;//�����ƊO���̋��E���Ԃ̋��������߂�
	lenPt.GetArrayPointInOut(aryPoint,aryPointOut,aryPointIn); //�O�����E���Ɠ������E�����쐬����

	CPoint ptS,ptE;
	double dbLen = aryPointOut.GetLength(aryPointIn,ptS,ptE);
	SizeDbl size;
	if( dbLen < DBL_MAX ){
		size.cx = dbLen;
		size.cy = dbLen;
		m_ckResult.SetLine(ptS,ptE);
	}
	return size;
}

//�������E���ƊO�����E���Ԃ̋�������
SizeDbl CharRecog::CheckIn2Out1(TCHAR tch,CPoint &pt,ArrayPoint& aryPoint)
{
	ArrayPoint aryPointOut;	//�O�����E��
	ArrayPoint aryPointIn1;	//�������E��
	ArrayPoint aryPointIn2;	//�������E��

	LengthPoint lenPt;//�����ƊO���̋��E���Ԃ̋��������߂�
	lenPt.GetArrayPointInOut(aryPoint,aryPointOut,aryPointIn1,aryPointIn2); //�O�����E���Ɠ������E�����쐬����

	CPoint ptS1,ptE1;
	double dbLen1 = aryPointOut.GetLength(aryPointIn1,ptS1,ptE1);
	CPoint ptS2,ptE2;
	double dbLen2 = aryPointOut.GetLength(aryPointIn2,ptS2,ptE2);
	SizeDbl size;
	if( dbLen1 < dbLen2 ){
		size.cx = dbLen1;
		size.cy = dbLen1;
		m_ckResult.SetLine(ptS1,ptE1);
	}else if(dbLen2 <= dbLen1){
		size.cx = dbLen2;
		size.cy = dbLen2;
		m_ckResult.SetLine(ptS2,ptE2);
	}
	return size;
}

//�ŒZ�����̔z��ԍ����擾
int CharRecog::GetMinLengthIndex(CPoint &pt)
{
	int i=0;
	int imax=m_aryPoint.GetSize();
	for(;i<imax;i++){
		if(m_aryPoint[i]==pt){ //������W�����݂��鎞
			return i;
		}
	}
	//������W�����݂��Ȃ���
	int nRet = 0;
	double dbMin = INT_MAX;
	for(i=0;i<imax;i++){
		double dbLen = ::GetLength(pt,m_aryPoint[i]); //���̗��[���܂�
		if( dbLen < dbMin ){ //��菬�����l�ɕύX
			dbMin = dbLen;
			nRet = i;
		}
	}
	return nRet;
}

//���E����̔z����������擾
int CharRecog::GetIndexLen(CPoint &pt1,CPoint &pt2)
{
	int nIndex1 = GetMinLengthIndex(pt1);
	int nIndex2 = GetMinLengthIndex(pt2);

	int nDelta1 = abs( nIndex1-nIndex2 );
	int nDelta2 = abs(nDelta1 - m_aryPoint.GetSize());
	return min(nDelta1,nDelta2); //index�ԋ������擾
}

//������E�����ւ̃G�b�W�̐������߂�
int CharRecog::GetEdgeNumLeftToRight(int nRnum,int nStartX,AryLine *pOutput, AryLine &aryLineH,Label &label,CPoint &ptTL)
{
	if( pOutput != NULL ){ //Clear Output
		pOutput->RemoveAll();
	}
	int nEdgeNum = 0;
	int nCx = label.GetWidth();
	int nCy = label.GetHeight();	
	CPoint ptNow(-1,-1);//����In Point	
	CPoint ptPrev(-1,-1);//�O��In Point	
	BYTE* pBits = label.GetDstBuf(); //���x�����O���ʎ擾
	int nLastY = 0;
	CLine linePrev,lineNow;
	BOOL isEdge = TRUE; //�G�b�W�ł����H

	SizeDbl size = GetDefLineWidth(nRnum);
	double dbEdgeLen = max(size.cx,MIN_EDGE_LEN); //�G�b�W���苗��

	for(int y=0;y<nCy;y++){
		BYTE *pFind = pBits + y * nCx + nStartX; //�����J�n�A�h���X
		ptNow.y = y;
		for(int x=nStartX,xmax=nCx-1;x<xmax;x++,pFind++){
			ptNow.x = x;
			if( *pFind==MAXBYTE && *(pFind+1)==MAXBYTE){ //In Point����������
				isEdge = FALSE;
				lineNow.Clear(); //���C����̓_���N���A
				if( ptPrev.x >= 0 && abs(nLastY-y) >= 2){ //�����\ && �G�b�W�̘A���s��					
					if( aryLineH.GetLine(linePrev,ptPrev)  //��O�̍��W���擾
					&&  aryLineH.GetLine(lineNow ,ptNow) ){ //�w����W�̐��������擾
						if(  abs(ptNow.x-ptPrev.x) > dbEdgeLen){ //���C���̊J�n�_������Ă���Ƃ�
							nEdgeNum++;
							nLastY = y;
							isEdge = TRUE;
							if( pOutput != NULL ){
								pOutput->RemoveY(y-1); //�G�b�W�ߕӍ폜
								pOutput->RemoveY(y-2); //�G�b�W�ߕӍ폜
							}
						}
					}
				}else if(ptPrev.x < 0){ //�J�n�|�C���g
					nEdgeNum++;
					nLastY = y;
					isEdge = TRUE;
				}
				if( !isEdge && abs(nLastY-y) >= 2){ //�G�b�W�ł͂Ȃ�&& �G�b�W�ߕӂł��Ȃ�������
					if( pOutput != NULL && lineNow.IsValid() ){
						pOutput->Add(lineNow); //���C���ǉ�
					}
				}
				ptPrev = ptNow;
				break;
			}
		}
	}
	if( abs(ptPrev.y-nLastY) >= 2 ){
		nEdgeNum++; //�G�b�W�̐�
		if( pOutput != NULL ){
			pOutput->RemoveLast();
			pOutput->RemoveLast();
		}
	}
	return nEdgeNum;
}

//�E���獶�����ւ̃G�b�W�̐������߂�
int CharRecog::GetEdgeNumRightToLeft(int nRnum,int nStartX,AryLine *pOutput,AryLine &aryLineH,Label &label,CPoint &ptTL)
{
	if( pOutput != NULL ){ //Clear Output
		pOutput->RemoveAll();
	}
	int nEdgeNum = 0;
	int nCx = label.GetWidth();
	int nCy = label.GetHeight();	
	CPoint ptNow(-1,-1);//����In Point	
	CPoint ptPrev(-1,-1);//�O��In Point	
	BYTE* pBits = label.GetDstBuf(); //���x�����O���ʎ擾
	int nLastY = 0;
	CLine linePrev,lineNow;
	BOOL isEdge = TRUE; //�G�b�W�ł����H

	SizeDbl size = GetDefLineWidth(nRnum);
	double dbEdgeLen = max(size.cx,MIN_EDGE_LEN); //�G�b�W���苗��

	for(int y=0;y<nCy;y++){
		BYTE *pFind = pBits + y * nCx + nStartX; //�����J�n�A�h���X
		ptNow.y = y;
		for(int x=nStartX;x>0;x--,pFind--){
			ptNow.x = x;
			if( *pFind==MAXBYTE && *(pFind-1)==MAXBYTE ){ //In Point����������
				isEdge = FALSE;
				lineNow.Clear(); //���C����̓_���N���A
				if( ptPrev.x >= 0 && abs(nLastY-y) >= 2){ //�����\ && �G�b�W�̘A���s��					
					if( aryLineH.GetLine(linePrev,ptPrev)  //��O�̍��W���擾
					&&  aryLineH.GetLine(lineNow ,ptNow) ){ //�w����W�̐��������擾
						if(  abs(ptNow.x-ptPrev.x) > dbEdgeLen ){ //���C���̊J�n�_������Ă���Ƃ�
							nEdgeNum++;
							nLastY = y;
							isEdge = TRUE;
							if( pOutput != NULL ){
								pOutput->RemoveY(y-1); //�G�b�W�ߕӍ폜
								pOutput->RemoveY(y-2); //�G�b�W�ߕӍ폜
							}
						}
					}
				}else if(ptPrev.x < 0){ //�J�n�|�C���g
					nEdgeNum++;
					nLastY = y;
					isEdge = TRUE;
				}
				if( !isEdge && abs(nLastY-y) >= 2){ //�G�b�W�ł͂Ȃ�&& �G�b�W�ߕӂł��Ȃ�������
					if( pOutput != NULL && lineNow.IsValid() ){
						pOutput->Add(lineNow); //���C���ǉ�
					}
				}
				ptPrev = ptNow;
				break;
			}
		}
	}
	if( abs(ptPrev.y-nLastY) >= 2 ){
		nEdgeNum++; //�G�b�W�̐�
		if( pOutput != NULL ){
			pOutput->RemoveLast();
			pOutput->RemoveLast();
		}
	}
	return nEdgeNum;
}

//�ォ�牺�����̃G�b�W��
int CharRecog::GetEdgeNumTopToBtm(int nRnum,AryLine *pOutput,AryLine &aryLineV,Label &label,CPoint &ptTL,int nYs/*=0*/)
{
	if( pOutput != NULL ){ //Clear Output
		pOutput->RemoveAll();
	}
	int nEdgeNum = 0;
	int nCx = label.GetWidth();
	int nCy = label.GetHeight();	
	CPoint ptNow(-1,-1);//����In Point	
	CPoint ptPrev(-1,-1);//�O��In Point	
	BYTE* pBits = label.GetDstBuf(); //���x�����O���ʎ擾
	int nLastX = 0;
	CLine linePrev,lineNow;
	BOOL isEdge = TRUE; //�G�b�W�ł����H
	SizeDbl size = GetDefLineWidth(nRnum);
	double dbEdgeLen = max(size.cy,MIN_EDGE_LEN); //�G�b�W���苗��
	for(int x=0;x<nCx;x++){
		BYTE *pFind = pBits + nYs*nCx + x; //�����J�n�A�h���X
		ptNow.x = x;
		for(int y=nYs,ymax=nCy-1;y<ymax;y++,pFind+=nCx){
			ptNow.y = y;
			if( *pFind==MAXBYTE && *(pFind+nCx)==MAXBYTE ){ //In Point����������
				isEdge = FALSE;
				lineNow.Clear(); //���C����̓_���N���A
				if( ptPrev.x >= 0 ){ //�����\ && �G�b�W�̘A���s��
					if( aryLineV.GetLine(linePrev,ptPrev)   //��O�̍��W���擾
					&&  aryLineV.GetLine(lineNow ,ptNow) ){ //�w����W�̍������擾						
						if(  abs(ptNow.y-ptPrev.y) > dbEdgeLen){ //���C���̊J�n�_������Ă���Ƃ�
							nEdgeNum++;
							nLastX = x;
							isEdge = TRUE;
							////////////////////////////////////////////FOR DEBUG/////////////////////
							//CPoint ptEdge = ptNow;
							//CRect rect(ptEdge.x+ptTL.x,ptEdge.y+ptTL.y,ptEdge.x+1+ptTL.x,ptEdge.y+1+ptTL.y);
							//GetView()->m_mgrVO.CreateRectObject(L"",rect,COLOR_RED,TRUE);					
							////////////////////////////////////////////FOR DEBUG/////////////////////
							if( pOutput != NULL ){
								pOutput->RemoveX(x-1); //�G�b�W�ߕӍ폜
								pOutput->RemoveX(x-2); //�G�b�W�ߕӍ폜
							}
						}
					}
				}else if(ptPrev.x < 0){ //�J�n�|�C���g
					nEdgeNum++;
					nLastX = x;
					isEdge = TRUE;
				}
				if( !isEdge && abs(nLastX-x) >= 2){ //�G�b�W�ł͂Ȃ��Ƃ�����
					if( pOutput != NULL && lineNow.IsValid() ){
						pOutput->Add(lineNow); //���C���ǉ�
					}
				}
				ptPrev = ptNow;
				break;
			}
		}
	}
	if( abs(ptPrev.x-nLastX) >= 2 ){
		nEdgeNum++; //�G�b�W�̐�
		if( pOutput != NULL ){
			int nXmax = pOutput->GetMaxX();
			pOutput->RemoveX(nXmax);
			pOutput->RemoveX(nXmax-1);
		}
	}
	return nEdgeNum;
}

//�����������̃G�b�W��
int CharRecog::GetEdgeNumBtmToTop(int nRnum,AryLine *pOutput,AryLine &aryLineV,Label &label,CPoint &ptTL)
{
	if( pOutput != NULL ){ //Clear Output
		pOutput->RemoveAll();
	}
	int nEdgeNum = 0;
	int nCx = label.GetWidth();
	int nCy = label.GetHeight();	
	CPoint ptNow(-1,-1);//����In Point	
	CPoint ptPrev(-1,-1);//�O��In Point	
	BYTE* pBits = label.GetDstBuf(); //���x�����O���ʎ擾
	int nLastX = 0;
	CLine linePrev,lineNow;
	BOOL isEdge = TRUE; //�G�b�W�ł����H
	SizeDbl size = GetDefLineWidth(nRnum);
	double dbEdgeLen = max(size.cy,MIN_EDGE_LEN); //�G�b�W���苗��
	for(int x=0;x<nCx;x++){
		BYTE *pFind = pBits + (nCy-1)*nCx + x; //�����J�n�A�h���X
		ptNow.x = x;
		for(int y=nCy-1;y>0;y--,pFind-=nCx){
			ptNow.y = y;
			if( *pFind==MAXBYTE && *(pFind-nCx)==MAXBYTE ){ //In Point����������
				isEdge = FALSE;
				lineNow.Clear(); //���C����̓_���N���A
				if( ptPrev.x >= 0 && abs(nLastX-x) >= 2 ){ //�����\ && �G�b�W�̘A���s��
					if( aryLineV.GetLine(linePrev,ptPrev)   //��O�̍��W���擾
					&&  aryLineV.GetLine(lineNow ,ptNow) ){ //�w����W�̍������擾
						SizeDbl size = GetDefLineWidth(nRnum);
						if(  abs(ptNow.y-ptPrev.y) > dbEdgeLen ){ //���C���̊J�n�_������Ă���Ƃ�
							nEdgeNum++;
							nLastX = x;
							isEdge = TRUE;
							////////////////////////////////////////////FOR DEBUG/////////////////////
							//CPoint ptEdge = ptNow;
							//CRect rect(ptEdge.x+ptTL.x,ptEdge.y+ptTL.y,ptEdge.x+1+ptTL.x,ptEdge.y+1+ptTL.y);
							//GetView()->m_mgrVO.CreateRectObject(L"",rect,COLOR_RED,TRUE);					
							////////////////////////////////////////////FOR DEBUG/////////////////////
							if( pOutput != NULL ){
								pOutput->RemoveX(x-1); //�G�b�W�ߕӍ폜
								pOutput->RemoveX(x-2); //�G�b�W�ߕӍ폜
							}
						}
					}
				}else if(ptPrev.x < 0){ //�J�n�|�C���g
					nEdgeNum++;
					nLastX = x;
					isEdge = TRUE;
				}
				if( !isEdge && abs(nLastX-x) >= 2){ //�G�b�W�ł͂Ȃ��Ƃ�����
					if( pOutput != NULL && lineNow.IsValid() ){
						pOutput->Add(lineNow); //���C���ǉ�
					}
				}
				ptPrev = ptNow;
				break;
			}
		}
	}
	if( abs(ptPrev.x-nLastX) >= 2 ){
		nEdgeNum++; //�G�b�W�̐�
		if( pOutput != NULL ){
			pOutput->RemoveLast();
			pOutput->RemoveLast();
		}
	}
	return nEdgeNum;
}

//�c�����Z���^�[���C�������؂���̐����J�E���g���܂�
BOOL CharRecog::CenterLineVCheck(int nRnum,TCHAR tch,Label &label,int nCheck)
{
	if( !IsInclination(nRnum) ){ //�΂ߕ����ȊO�̂Ƃ�����
		int nRet = 0;
		BYTE* pBits = label.GetDstBuf(); //���x�����O���ʎ擾
		int nCx = label.GetWidth();
		int nCy = label.GetHeight();
		int x = nCx / 2; //�Z���^�[���C��
		BYTE *pFind = pBits + x; //�����J�n�A�h���X
		for(int y=0;y<nCy;y++,pFind+=nCx){
			if( *pFind==MAXBYTE ){ //In Point����������			
				BOOL isFindOK = FALSE;
				for(;y<nCy;y++,pFind+=nCx){//Out Point ��
					if(!*pFind){ //Out Point����������
						isFindOK = TRUE;
						nRet++; //���̐��C���N�������g
						break;
					}
				}
				if(!isFindOK){//Out Point��������Ȃ�����
					nRet++; //�[�܂œ��B�������߂ɐ��̐��C���N�������g
				}
			}
		}
		if( nRet!=nCheck){
			GetOutputView()->Output(false,L"%c: CenterLine %d�{",tch,nRet);
			return FALSE;
		}
	}
	return TRUE; 
}

//�����̐����`�F�b�N
bool CharRecog::CheckSize(TCHAR tch,SizeDbl &size,double dbWmin,double dbLen)
{
	if(dbWmin <= 0 ){
		return false;
	}
	bool isOK = ((size.cx-0.0001) <= dbWmin); //�����̐����͊�l�ȏ�ł����H(0.0001�̈Ⴂ�܂ŋ��j

	CString strLen;
	strLen.Format(L"�����F��:%3.1f",dbLen);
	while( strLen.GetLength() < 10 ){
		strLen += ' '; //���p�󔒒ǉ�
	}

	CString strOut; //�o�̓��b�Z�[�W
	if(dbWmin >= 0 ){
		strOut.Format(L"%c: %s ��:%.2f",tch,strLen,dbWmin);
	}else{
		strOut.Format(L"%c: %s",tch,strLen);
	}
	GetOutputView()->Output(isOK,strOut.GetBuffer());
	return isOK;
}

//�w��z�񒆂̍ŏ����擾
int CharRecog::GetWminIndex(AryLine *pAryLine,double &dbWmin)
{
	dbWmin = 0; //�����l 
	int nIndex = -1;
	double dbMin = INT_MAX; //����
	for(int i=0,imax=pAryLine->GetSize();i<imax;i++){ //�擪�Ɩ������G�b�W�Ƃ��ĊO��
		double dbW = pAryLine->GetAt(i).GetLength();
		//TRACE("dbW %.1f\n",dbW);
		if( dbW < dbMin){//��菬�����l����������
			nIndex = i;
			dbMin = dbW; 
		}
	}
	if( nIndex >= 0 ){
		dbWmin = dbMin; //�ŏ���Set
	}
	return nIndex; //Index�ԍ��擾
}

//�폜�����擾
double CharRecog::GetDeleteWidth( const CRect &rect,double dbLen) 
{
	int nWidth = rect.Width();
	int nHeight= rect.Height();
	return min3(nWidth,nHeight,dbLen); 
}

//Point�̕\��
void CharRecog::ShowPoint(LPWSTR szName,CPoint &pt,Color color)
{
	CString strName;
	strName.Format(L"%s%d%d",szName,pt.x,pt.y);
	
	CRect rect(pt.x,pt.y,pt.x+1,pt.y+1);
	GetView()->m_mgrVO.CreateRectObject(strName,rect,color,true); //��`�`��
}

//������������
CheckNickData CharRecog::CheckNick(int nIndex,Label &label,CharElement &celem,CRect &rect,ArrayPoint& aryPoint,int nRnum,double dbLen,int nTh)
{
	CheckNickData data;
	data.m_tChar = celem.m_tChar;

	CPoint pt = rect.TopLeft();//���΍��W
	TCHAR tch = celem.m_tChar;
	SizeDbl size = GetLineWidthAll(nIndex,tch,nRnum); //�����I�Ȕ��f�ɂ�镶�����擾

	m_ckResult.Clear(); //�������ʃN���A
	m_ckResult.SetTL(pt); //����ݒ�

	label.FillDstBuf(0,MAXBYTE); //���x�����O���ʂ�255
	//label.ContractDst(); //�k�� (�ׂ����������j	
	//label.ExpandDst();   //�c�� 	

	COutputView *pOutput = GetOutputView();	
	double dbWmin = -1;//���̍ŏ��l
	double dbDelete = GetDeleteWidth( rect,size.cx ); //�폜����
	if( size.cx > 0 ){ //���w��͂���܂����H
		switch(tch){
			case '-': //LR
				dbWmin = TopToBtm(aryPoint,3,INT_MAX); //�ォ�牺
				break;
			case 'i': 
			case 'I':
			case 'l': //LR
				dbWmin = LeftToRight(aryPoint,3,INT_MAX); //������E����
				break;
			case '/': //LR + TB	Delete ����
				dbWmin = TopToBtm   (aryPoint,dbDelete,INT_MAX); //�ォ�牺
				dbWmin = LeftToRight(aryPoint,2,dbWmin ); //������E����
				break;
				break;
			case 'r'://LR+TB
			case '1':
				dbWmin = LeftToRight(aryPoint,dbDelete,INT_MAX); //������E����
				dbWmin = TopToBtm   (aryPoint,       2,dbWmin ); //�ォ�牺
				break;
			case 'X': //LR + RL
			case 'x':		
				dbWmin = LeftToRight(aryPoint,dbDelete,INT_MAX); //������E
				dbWmin = RightToLeft(aryPoint,dbDelete,dbWmin); //�E���獶
				break;
			case 'L': //LR + BT
				dbWmin = LeftToRight(aryPoint,dbDelete,INT_MAX); //������E
				dbWmin = BtmToTop   (aryPoint,dbDelete,dbWmin ); //�������
				break;
			case '7'://RL + TB
				dbWmin = RightToLeft(aryPoint,dbDelete,INT_MAX); //������E
				dbWmin = TopToBtm   (aryPoint,dbDelete,dbWmin ); //�ォ���
				break;
			case 'C': //LR + TB + BT(�����폜�j
			case 'c':
				{
					dbWmin = TopToBtm   (aryPoint,dbDelete,INT_MAX); //�ォ�牺
					dbWmin = BtmToTop   (aryPoint,dbDelete,dbWmin); //�������
					dbWmin = LeftToRight(aryPoint,dbDelete,dbWmin); //������E����
				}
				break;
			case 'f': //LR + TB + BT
			case 'F':
				{
					dbWmin = TopToBtm   (aryPoint,dbDelete,INT_MAX); //�ォ�牺
					dbWmin = BtmToTop   (aryPoint,dbDelete,dbWmin ); //�������
					dbWmin = LeftToRight(aryPoint,dbDelete,dbWmin); //������E����
				}
				break;
			case 'W':// HCenter
			case 'M':
			case 'm':
			case 'w':
				{
					dbWmin = LeftToRight  (aryPoint,dbDelete,INT_MAX); //������E����
					dbWmin = RightToLeft  (aryPoint,dbDelete,dbWmin); //������E����
					dbWmin = CenterToLeft (aryPoint,dbDelete,dbWmin); //�������獶					
					dbWmin = CenterToRight(aryPoint,dbDelete,dbWmin); //��������E
				}
				break;
			
			case 'E':
				if( CenterLineVCheck(nRnum,tch,label,3) ){ //�c�����Z���^�[���C���R�{					
					CRect rect = aryPoint.GetRect();//���͒��̈�
					int nCenterY = rect.CenterPoint().y;
					//�㉺�����ɕ���
					ArrayPoint aryPointTop;//�㔼��
					aryPointTop.Copy(aryPoint);
					aryPointTop.RangeY(0,nCenterY);
					ArrayPoint aryPointBtm;//������
					aryPointBtm.Copy(aryPoint);
					aryPointBtm.RangeY(nCenterY,rect.bottom);
					
					//�㔼��
					dbWmin = LeftToRight(aryPointTop,dbDelete,INT_MAX); //������E����
					dbWmin = TopToBtm(aryPointTop,   dbDelete,dbWmin); //�ォ�牺

					//������
					dbWmin = LeftToRight(aryPointBtm,dbDelete,dbWmin); //������E����
					dbWmin = BtmToTop   (aryPointBtm,dbDelete,dbWmin); //�������
					
					//�����̂�
					ArrayPoint aryPointC;//����
					aryPointC.Copy(aryPoint);
					aryPointC.RangeY(nCenterY-size.cy*1.5,nCenterY+size.cy*1.5);
					dbWmin = TopToBtm(aryPointC,2,dbWmin); //�ォ�牺
				}
				break;
			case '3':
				if( CenterLineVCheck(nRnum,tch,label,3) ){ //�c�����Z���^�[���C���R�{					
					CRect rect = aryPoint.GetRect();//���͒��̈�
					int nCenterY = rect.CenterPoint().y;
					//�㉺�����ɕ���
					ArrayPoint aryPointTop;//�㔼��
					aryPointTop.Copy(aryPoint);
					aryPointTop.RangeY(0,nCenterY);
					ArrayPoint aryPointBtm;//������
					aryPointBtm.Copy(aryPoint);
					aryPointBtm.RangeY(nCenterY,rect.bottom);
					
					//�㔼��
					dbWmin = RightToLeft(aryPointTop,dbDelete,INT_MAX); //������E����
					dbWmin = TopToBtm   (aryPointTop,       2,dbWmin); //�ォ�牺

					//������
					dbWmin = RightToLeft(aryPointBtm,dbDelete,dbWmin); //������E����
					dbWmin = BtmToTop   (aryPointBtm,       2,dbWmin); //�������

					//�����̂�
					ArrayPoint aryPointC;//����
					aryPointC.Copy(aryPoint);
					aryPointC.RangeY(nCenterY-size.cy*1.5,nCenterY+size.cy*1.5);
					dbWmin = TopToBtm(aryPointC,2,dbWmin); //�ォ�牺
				}
				break;
			case 'a'://IO + RL
			case 'd':
			case 'q':
				{
					SizeDbl sizeIO = CheckInOut(tch,pt,aryPoint);  //��������
					dbWmin = RightToLeft(aryPoint,dbDelete,sizeIO.cx); //������E����
				}
				break;
			case 'P':
			case 'p'://IO + LR
				{
					SizeDbl sizeIO = CheckInOut(tch,pt,aryPoint);  //��������
					dbWmin = LeftToRight(aryPoint,dbDelete,sizeIO.cx); //������E����
				}
				break;
						
			case 'Q': //IO + BT
				{
					SizeDbl sizeIO = CheckInOut(tch,pt,aryPoint);  //��������
					dbWmin = BtmToTop(aryPoint,dbDelete,sizeIO.cy); //�������
				}
				break;
			case 'A': //IO + LR + RL
			case 'R':
				{
					SizeDbl sizeIO = CheckInOut(tch,pt,aryPoint);  //��������
					dbWmin = LeftToRight(aryPoint,dbDelete,sizeIO.cx); //������E����
					dbWmin = RightToLeft(aryPoint,dbDelete,dbWmin   ); //������E����
				}
				break;			
			case 'e': //IO + LR + BT
				{
					SizeDbl sizeIO = CheckInOut(tch,pt,aryPoint);  //��������
					dbWmin = LeftToRight(aryPoint,dbDelete,sizeIO.cx); //������E����
					dbWmin = BtmToTop   (aryPoint,dbDelete,dbWmin); //�������
				}
				break;
			case '4': //IO + RL + BT
				if(CenterLineVCheck(nRnum,tch,label,2)){ //�c�����Z���^�[���C��2�{
					SizeDbl sizeIO = CheckInOut(tch,pt,aryPoint);  //��������
					dbWmin = RightToLeft(aryPoint,dbDelete,sizeIO.cx); //������E����
					dbWmin = TopToBtm   (aryPoint,       2,dbWmin); //�ォ�牺
				}
				break;
			case '6': //IO + LR + TB
				{
					//�����ƊO��
					ArrayPoint aryPointOut;	//�O�����E��
					ArrayPoint aryPointIn;	//�������E��
					LengthPoint lenPt;//�����ƊO���̋��E���Ԃ̋��������߂�
					lenPt.GetArrayPointInOut(aryPoint,aryPointOut,aryPointIn); //�O�����E���Ɠ������E�����쐬����
					SizeDbl sizeIO;
					CPoint ptS,ptE; //�ŏ�����Point
					dbWmin = aryPointIn.GetLength(aryPointOut,ptS,ptE); //In Out�Ԃ̍ŏ��������擾
					m_ckResult.SetLine(ptS,ptE);

					//��������
					dbWmin = LeftToRight(aryPoint,dbDelete,dbWmin); 

					//�㑤����
					//�������E���������폜
					int nRangeY = aryPointIn.GetTop().y; //�������E�����
					ArrayPoint aryPointTop;//�㔼��
					aryPointTop.Copy(aryPoint);
					aryPointTop.RangeY(0,nRangeY);
					dbWmin = TopToBtm(aryPointTop,dbDelete,dbWmin); //�ォ�牺
				}
				break;
			case '9'://IO + RL + BT
				if(CenterLineVCheck(nRnum,tch,label,3)){ //�c�����Z���^�[���C��3�{
					//�����ƊO��
					ArrayPoint aryPointOut;	//�O�����E��
					ArrayPoint aryPointIn;	//�������E��
					LengthPoint lenPt;//�����ƊO���̋��E���Ԃ̋��������߂�
					lenPt.GetArrayPointInOut(aryPoint,aryPointOut,aryPointIn); //�O�����E���Ɠ������E�����쐬����

					CPoint ptS,ptE; //x�����ŏ�����Point
					dbWmin = aryPointIn.GetLength(aryPointOut,ptS,ptE); //In Out�Ԃ�x�����̍ŏ��l���擾
					m_ckResult.SetLine(ptS,ptE);

					//�E������
					dbWmin = RightToLeft(aryPoint,dbDelete,dbWmin); 

					//�㑤����
					//�������E���������폜
					int nBtm = aryPointOut.GetBottom().y; //�O�����E������
					int nTop = aryPointIn.GetBottom().y; //�������E������
					ArrayPoint aryPointTop;//�㔼��
					aryPointTop.Copy(aryPoint);
					aryPointTop.RangeY(nTop,nBtm);
					dbWmin = BtmToTop(aryPointTop,dbWmin,dbWmin); //�������
				}
				break;
			case '0': //���E���Ԍ���
			case 'D': //IO
			case 'O':
			case 'o':
				if( CenterLineVCheck(nRnum,tch,label,2) ){ //�c�����Z���^�[���C��2�{
					SizeDbl sizeIO = CheckInOut(tch,pt,aryPoint);  //��������
					dbWmin = sizeIO.cx;
				}
				break;
			case 'B': //IO 2��
			case '8':
				if( CenterLineVCheck(nRnum,tch,label,3) ){ //�c�����Z���^�[���C��3�{
					SizeDbl sizeIO = CheckIn2Out1(tch,pt,aryPoint);
					dbWmin = sizeIO.cx;
				}
				break;
			case '2': //TB + BT
			case 'Z': //�㔼�� RL
			case 'z': //������ LR
				{
					//�㉺�����ɕ���
					CRect rect = aryPoint.GetRect();//���͒��̈�
					int nCenterY = rect.CenterPoint().y;
					ArrayPoint aryPointTop;//�㔼��
					aryPointTop.Copy(aryPoint);
					aryPointTop.RangeY(0,nCenterY);
					ArrayPoint aryPointBtm;//������
					aryPointBtm.Copy(aryPoint);
					aryPointBtm.RangeY(nCenterY,rect.bottom);
					dbWmin = LeftToRight(aryPointBtm,3,INT_MAX); //������������E
					dbWmin = BtmToTop(aryPointBtm,   3,dbWmin);  //�������������
					dbWmin = RightToLeft(aryPointTop,3,dbWmin);	 //�㔼���E���獶
					dbWmin = TopToBtm(aryPointTop,dbDelete,dbWmin);//�㔼���ォ�牺
				}
				break;
			case '5':
				{										
					//�㉺�����ɕ���
					CRect rect = aryPoint.GetRect();//���͒��̈�
					int nCenterY = rect.CenterPoint().y;
					ArrayPoint aryPointTop;//�㔼��
					aryPointTop.Copy(aryPoint);
					aryPointTop.RangeY(0,nCenterY);
					ArrayPoint aryPointBtm;//������
					aryPointBtm.Copy(aryPoint);
					aryPointBtm.RangeY(nCenterY,rect.bottom);
					dbWmin = LeftToRight(aryPointTop,dbDelete,INT_MAX);	//�㔼��������E
					dbWmin = TopToBtm(aryPointTop,3,dbWmin );			//�㔼���ォ�牺
					dbWmin = RightToLeft(aryPointBtm,dbDelete,dbWmin);	//�������E���獶
					dbWmin = BtmToTop(aryPointBtm,3,dbWmin);			//�������������
				}
				break;
			case 'S':
			case 's':
				{
					//3����
					CRect rect = aryPoint.GetRect();//���͒��̈�
					int n3_1 = rect.Height() / 3; //3����1�̍���

					ArrayPoint aryPoint1_3;//3����1��
					ArrayPoint aryPoint2_3;//���S
					ArrayPoint aryPoint3_3;//3����1��
					aryPoint1_3.Copy(aryPoint);
					aryPoint2_3.Copy(aryPoint);
					aryPoint3_3.Copy(aryPoint);
					aryPoint1_3.RangeY(rect.top,rect.top+n3_1);
					aryPoint2_3.RangeY(rect.top+n3_1,rect.top+n3_1*2);
					aryPoint3_3.RangeY(rect.top+n3_1*2,rect.bottom);

					dbWmin = TopToBtm(aryPoint1_3,dbDelete,INT_MAX);	//�ォ�牺
					dbWmin = LeftToRight(aryPoint2_3,dbDelete,dbWmin);	//������E
					dbWmin = BtmToTop(aryPoint3_3,dbDelete,dbWmin);		//�������

					//S�̍��E�Ȃ����Ă��镔����
					dbWmin = LeftToRight(aryPoint1_3,dbDelete,dbWmin);	//������E
					dbWmin = RightToLeft(aryPoint3_3,dbDelete,dbWmin);	//�E���獶

					//S�̍��E�����Ă��镔����
					CPoint ptRight  = aryPoint1_3.GetRight();	//�ł��E�̃|�C���g
					CPoint ptBtm1_3 = aryPoint1_3.GetBottom();
					int nDelete = ptBtm1_3.y - ptRight.y;
					if( nDelete > 0 ){
						dbWmin = RightToLeft(aryPoint1_3,nDelete,dbWmin);	//�E���獶						
					}
					CPoint ptLeft  = aryPoint3_3.GetLeft();	//�ł��E�̃|�C���g
					CPoint ptBtm3_3 = aryPoint3_3.GetBottom();					
					nDelete = ptBtm3_3.y - ptLeft.y;
					if( nDelete > 0 ){
						dbWmin = LeftToRight(aryPoint3_3,nDelete,dbWmin);	//������E						
					}
				}
				break;
			case 'N': 
				{
					dbWmin = LeftToRight(aryPoint,dbDelete,INT_MAX); //������E
					dbWmin = RightToLeft(aryPoint,dbDelete,dbWmin); //�E���獶

					//���E�����ɕ���
					CRect rect = aryPoint.GetRect();//���͒��̈�
					int nCenterX = rect.CenterPoint().x;
					ArrayPoint aryPointLeft;//������
					aryPointLeft.Copy(aryPoint);
					aryPointLeft.RangeX(0,nCenterX+size.cx);
					ArrayPoint aryPointRight;//�E����
					aryPointRight.Copy(aryPoint);
					aryPointRight.RangeX(nCenterX-size.cx,rect.right);
					dbWmin = TopToBtm(aryPointLeft ,dbDelete,INT_MAX); //�������ォ�牺
					dbWmin = BtmToTop(aryPointRight,dbDelete,dbWmin ); //�E�����������
				}
				break;
			case 'h': //LR + RL + TB
			case 'n':
				dbWmin = LeftToRight(aryPoint,dbDelete,INT_MAX); //������E
				dbWmin = RightToLeft(aryPoint,dbDelete,dbWmin); //�E���獶
				dbWmin = TopToBtm(aryPoint,dbDelete,dbWmin); //�ォ�牺
				break;
			case 'J':
			case 'j':
				{
					dbWmin = TopToBtm(aryPoint,dbDelete,dbWmin);			//�㔼���ォ�牺
					dbWmin = BtmToTop(aryPoint,dbDelete,dbWmin);			//�������������

					//���E�����ɕ���
					CRect rect = aryPoint.GetRect();//���͒��̈�
					int nCenterX = rect.CenterPoint().x;
					
					ArrayPoint aryPointLeft;//������
					aryPointLeft.Copy(aryPoint);
					aryPointLeft.RangeX(0,nCenterX);
					ArrayPoint aryPointRight;//�E����
					aryPointRight.Copy(aryPoint);
					aryPointRight.RangeX(nCenterX,rect.right);
					dbWmin = LeftToRight(aryPointLeft ,dbDelete,INT_MAX);	//�㔼��������E
					dbWmin = RightToLeft(aryPointRight,dbDelete,dbWmin);	//�������E���獶
				}
				break;
			case 'U': //LR + RL + BT
			case 'u':
			case 'V':
			case 'v':
			case 'y':
				dbWmin = LeftToRight(aryPoint,dbDelete,INT_MAX); //������E
				dbWmin = RightToLeft(aryPoint,dbDelete,dbWmin); //�E���獶
				dbWmin = BtmToTop(aryPoint,dbDelete,dbWmin); //�������
				break;
			case 'T':
				{
					dbWmin = TopToBtm(aryPoint,dbDelete,INT_MAX); //�ォ�牺

					//�������擾
					CRect rect = aryPoint.GetRect();//���͒��̈�
					int nCenterY = rect.CenterPoint().y;
					ArrayPoint aryPointBtm;//������
					aryPointBtm.Copy(aryPoint);
					aryPointBtm.RangeY(nCenterY,rect.bottom);
					CPoint ptLeft = aryPointBtm.GetLeft(); //�������̒��ł̍��[�擾

					ArrayPoint aryPointCpy;//���E���̃R�s�[�쐬
					aryPointCpy.Copy(aryPoint);

					//�㉺�����߂�
					int nTop = aryPointCpy.GetTop().y;	  //�ł���̓_���擾
					int nBtm = aryPointCpy.GetBottom().y; //�ł����̓_���擾
					aryPoint.RangeY(nTop+size.cy,nBtm-size.cy); //�w��͈݂͂̂�Point�ɕύX �����̔����������s�\�̈�Ƃ���

					ArrayPoint aryConnect;
					ArrayPoint aryNotConnect;
					aryPoint.CollectConnectPoint( ptLeft ,aryConnect); //���[���W�Ƃ̗א�Point�̂�
					aryPoint.CollectExceptPoint(aryConnect,aryNotConnect); //�א�Point�ȊO�����W
					aryNotConnect.SelectConnectPoint( (int)size.cy ); //�w�萔�ڑ�Point�݂̂�I��

					CRect rcLeft = aryConnect.GetRect(); //�אڗ̈�					
					//�אڗ̈�̉E�������ɂ���
					aryNotConnect.RangeX( rcLeft.left,rect.right);
					CPoint ptS,ptE;
					double dbLen = aryConnect.GetLength(aryNotConnect,ptS,ptE);
					if( dbLen < dbWmin){ //��������
						m_ckResult.SetLine(ptS,ptE);
						dbWmin = dbLen;
					}
				}
				break;
			//Default ���������`�F�b�N
			case 'K':
			case 'k':
			case 'G':
			case 'H':
			case 'Y':
			case 't': 
				dbWmin = LeftToRight(aryPoint,dbDelete,INT_MAX); //������E
				dbWmin = RightToLeft(aryPoint,dbDelete,dbWmin); //�E���獶
				dbWmin = TopToBtm(aryPoint   ,dbDelete,dbWmin); //�ォ�牺
				dbWmin = BtmToTop(aryPoint   ,dbDelete,dbWmin); //�������
				break;
			case '��':
			case L'��':
				{
					data.m_isOK = true;	
					return data;
				}
				break;
		}
	}
	//m_dibGray��m_ckResult�̌v�Z�ǉ�
	CPoint &ptS = m_ckResult.m_line.m_ptS;
	CPoint &ptE = m_ckResult.m_line.m_ptE;	
	if(dbWmin > 0){
		//CString strFilename;
		//strFilename.Format(L"c:\\tmp\\%d_%d_%d.bmp",nRnum,ptS.x,ptS.y);
		//m_dibGray.SaveFile(strFilename);

		PointDbl vec; ;//�����x�N�g��
		vec.x = (ptS.x-ptE.x)/dbWmin;
		vec.y = (ptS.y-ptE.y)/dbWmin;
		BYTE byDataS  = MAXBYTE - m_dibGray.GetValue(ptS); //�w����W�̒l�擾
		double dbDataS= MAXBYTE - m_dibGray.GetAprxVal(ptS.x+vec.x,ptS.y+vec.y);
		if( byDataS > dbDataS && byDataS > nTh && nTh > dbDataS ){ //�������l�ɂ������g��
			double dbS1 = byDataS-nTh; //�������l�]��
			double dbS2 = nTh-dbDataS; //�O���]��
			dbWmin += dbS1/(dbS1+dbS2);			
		}
		BYTE byDataE  = MAXBYTE - m_dibGray.GetValue(ptE); //�w����W�̒l�擾
		double dbDataE= MAXBYTE - m_dibGray.GetAprxVal(ptE.x-vec.x,ptE.y-vec.y);		
		if( byDataE > dbDataE && byDataE > nTh && nTh > dbDataE ){ //�������l�ɂ������g��
			double dbE1 = byDataE-nTh;//�������l�]��
			double dbE2 = nTh-dbDataE;//�O���]��
			dbWmin += dbE1/(dbE1+dbE2);			
		}
	}

	data.m_isOK = CheckSize(tch,size,dbWmin,dbLen);		
	if( data.m_isOK ){ //����������l�̎�
		SetMinWidth(tch,nRnum,dbWmin); //�ŏ����������ʓo�^
	}else{	//�����ُ�l�̎�
		m_ckResult.ShowMinLine(size); //�ŏ����C���̕\��	
	}
	return data;
}

#endif