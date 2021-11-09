#include "../stdafx.h"
#include "ArrayPoint.h"
#include "typedef.h"
#include "functions.h"
#include "GlobalMgr.h"
#include "../Script/App.h"

ArrayPoint& ArrayPoint::operator=(const ArrayPoint &t)
{
	this->Copy(t);
	return *this;
}

int ArrayPoint::Find(CPoint pt)
{
	for(int i=0;i<GetSize();i++){
		if( GetAt(i) == pt ){
			return i;
		}
	}	
	return -1;
}
INT_PTR ArrayPoint::AddPoint(int x,int y)
{
	CPoint pt(x,y);
	return this->Add(pt);
}

//�ł����̓_���擾
CPoint ArrayPoint::GetLeft()	
{
	CPoint ptRet(0,0); //Default�߂�l
	int imax=(int)GetSize(); //�T�C�Y�`�F�b�N
	if( imax > 0 ){
		ptRet.x = INT_MAX;//�ŏ��l
		for(int i=0;i<imax;i++){ 
			CPoint &pt = GetAt(i);
			if( pt.x < ptRet.x && pt.x >= 0){ //��菬�������W�����߂�
				ptRet = pt;
			}
		}
	}
	return ptRet;
}
//�ł��E�̓_���擾
CPoint ArrayPoint::GetRight()
{
	CPoint ptRet(0,0); //Default�߂�l
	int imax=(int)GetSize(); //�T�C�Y�`�F�b�N
	if( imax > 0 ){
		for(int i=0;i<imax;i++){ 
			CPoint &pt = GetAt(i);
			if( ptRet.x <= pt.x ){
				ptRet = pt;
			}
		}
	}
	return ptRet;
}

//�ł���̓_���擾
CPoint ArrayPoint::GetTop()	
{
	CPoint ptRet(0,0); //Default�߂�l
	int imax=(int)GetSize(); //�T�C�Y�`�F�b�N
	if( imax > 0 ){
		ptRet.y = INT_MAX;
		for(int i=0;i<imax;i++){
			CPoint &pt = GetAt(i);
			if( pt.y < ptRet.y && pt.y >= 0){ //��菬�����l�Œu��������
				ptRet = pt;
			}
		}
	}
	return ptRet;
}

//�ł����̓_���擾	
CPoint ArrayPoint::GetBottom() 
{
	CPoint ptRet(0,0); //Default�߂�l
	int imax=(int)GetSize(); //�T�C�Y�`�F�b�N
	if( imax > 0 ){
		for(int i=0;i<imax;i++){
			CPoint &pt = GetAt(i);
			if( ptRet.y <= pt.y ){
				ptRet = pt;
			}
		}
	}
	return ptRet;
}

//�_�z�񂩂��`���擾
CRect ArrayPoint::GetRect()
{
	CRect rcRet(INT_MAX,INT_MAX,INT_MIN,INT_MIN);
	for(int i=0,imax=(int)GetSize();i<imax;i++){ 
		CPoint &pt = GetAt(i);
		if( 0 <= pt.x && pt.x < rcRet.left ){
			rcRet.left = pt.x;
		}
		if(rcRet.right < pt.x){
			rcRet.right = pt.x;
		}
		if(0 <= pt.y && pt.y < rcRet.top){
			rcRet.top = pt.y;
		}
		if(rcRet.bottom < pt.y){
			rcRet.bottom = pt.y;
		}
	}
	return rcRet;
}
//�w����W�Ƃ̍ŒZ�����ƍŒZ�����ƂȂ�Point���擾
CPoint ArrayPoint::GetLengthPoint(CPoint& pt,double &dbLmin)
{
	CPoint ptRet(0,0);
	dbLmin = DBL_MAX; //�ŏ�����
	int nImin = -1;
	for(int i=0,imax=(int)GetSize();i<imax;i++){
		double dbLen = ::GetLength(pt,GetAt(i))+1; //���̗��[���܂�
		if( dbLen < dbLmin ){
			dbLmin = dbLen;
			nImin = i;
		}
	}
	if( nImin >= 0 ){
		ptRet = GetAt(nImin);
	}
	return ptRet;
}
//�w����W�Ƃ̍ŒZ���������߂�
double ArrayPoint::GetLength(CPoint& pt) 
{
	double dbLen;
	GetLengthPoint(pt,dbLen);
	return dbLen;
}

//�ŒZ�����擾
double ArrayPoint::GetLength(ArrayPoint &ptAry,CPoint &ptSmin,CPoint &ptEmin)
{
	double dbLmin = DBL_MAX;
	double dbLen;
	CPoint ptS,ptE;
	for(int i=0,imax=(int)GetSize();i<imax;i++){
		ptS = GetAt(i);
		ptE = ptAry.GetLengthPoint(ptS,dbLen);
		if( dbLen < dbLmin ){//�ŏ�����
			ptSmin = ptS; //�ʒu�L�^
			ptEmin = ptE;
			dbLmin = dbLen; //�����L�^
		}
	}
	return dbLmin; //�ŒZ����
}

//�w��x���W����
CPoint ArrayPoint::FindX(int x) 
{	
	for(int i=0,imax=(int)GetSize();i<imax;i++){ 
		CPoint &pt = GetAt(i);
		if(pt.x == x){
			return pt;
		}
	}
	return CPoint(-1,-1);
}
CPoint ArrayPoint::FindX(int nIndex,int x) 
{	
	for(int i=nIndex,imax=(int)GetSize();i<imax;i++){ 
		CPoint &pt = GetAt(i);
		if(pt.x == x){
			return pt;
		}
	}
	return CPoint(-1,-1);
}

//�w��y���W����
CPoint ArrayPoint::FindY(int y) 
{
	CPoint ptRet(-1,-1);
	for(int i=0,imax=(int)GetSize();i<imax;i++){
		CPoint &pt = GetAt(i);
		if(pt.y == y){
			return pt;
		}
	}
	return CPoint(-1,-1);
}
//�w��y���W����
CPoint ArrayPoint::FindY(int nIndex,int y) 
{
	CPoint ptRet(-1,-1);
	for(int i=nIndex,imax=(int)GetSize();i<imax;i++){
		CPoint &pt = GetAt(i);
		if(pt.y == y){
			return pt;
		}
	}
	return CPoint(-1,-1);
}

//�w��x���W�̓_�擾
int ArrayPoint::CollectPointX(int nX,ArrayPoint &ptCollect)
{
	int nCnt = 0;	
	for(int i=0,imax=(int)GetSize();i<imax;i++){ 
		CPoint &pt = GetAt(i);
		if(pt.x == nX){
			ptCollect.Add(pt);
			nCnt++;
		}
	}
	return nCnt;
}
//�w��y���W�̓_�擾
int ArrayPoint::CollectPointY(int nY,ArrayPoint &ptCollect)
{
	int nCnt = 0;	
	for(int i=0,imax=(int)GetSize();i<imax;i++){ 
		CPoint &pt = GetAt(i);
		if(pt.y == nY){
			ptCollect.Add(pt);
			nCnt++;
		}
	}
	return nCnt;
}

//�w��Point��X���W���w��l�ɐݒ�
void ArrayPoint::SetX(int x,ArrayPoint &ptCollect)
{
	for(int i=0;i<ptCollect.GetSize();i++){ //���W�����_���폜
		int nIndex = Find( ptCollect[i] );
		if( nIndex >= 0 ){
			GetAt(nIndex).x = -1; //�I�_�ɐ؂�ւ�
		}
	}
}

//x���W�Ń\�[�g
void ArrayPoint::SortX() 
{
	vector<CPoint> vPoint;
	int imax = (int)GetSize();
	for(int i=0;i<imax;i++){
		vPoint.push_back( GetAt(i) );
	}
	::sort(vPoint.begin(),vPoint.end(),SortPointX());
	
	RemoveAll();//clear
	for(int i=0;i<imax;i++){//���ёւ�
		Add( vPoint[i] );
	}
}

//y���W�Ń\�[�g
void ArrayPoint::SortY() 
{
	vector<CPoint> vPoint;
	int imax = (int)GetSize();
	for(int i=0;i<imax;i++){
		vPoint.push_back( GetAt(i) );
	}
	::sort(vPoint.begin(),vPoint.end(),SortPointY());
	
	RemoveAll();//clear
	for(int i=0;i<imax;i++){//���ёւ�
		Add( vPoint[i] );
	}
}

//x���W�ɂ����2�ɕ�����
void ArrayPoint::SplitX(int nX,ArrayPoint &aryL,ArrayPoint &aryR)
{	
	for(int i=0;i<GetSize();i++){
		CPoint &pt = GetAt(i);
		if( nX < pt.x ){ //�E���쐬
			aryR.Add(pt);
		}else{ //�����쐬
			aryL.Add(pt);
		}
	}		
}

//���_���W�Ƃ̗א�Point�̂ݎ��W
void ArrayPoint::CollectConnectPoint(CPoint &pt,ArrayPoint &aryCollect) 
{
	aryCollect.RemoveAll();
	int imax = (int)GetSize();
	aryCollect.Add(pt);
	for(int i=0;i<imax;i++){ //�������ɐڑ�����
		CPoint &ptCheck = GetAt(i);
		if( aryCollect.Find(ptCheck) < 0 && aryCollect.IsConnect(ptCheck) ){ //�ڑ��m�F
			aryCollect.Add(ptCheck);
		}
	}
	for(int i=imax-1;i>=0;i--){ //�t�����ɐڑ�����
		CPoint &ptCheck = GetAt(i);
		if( aryCollect.Find(ptCheck) < 0 && aryCollect.IsConnect(ptCheck) ){ //�ڑ��m�F
			aryCollect.Add(ptCheck);
		}
	}
}

//�א�Point�ȊO�����W
void ArrayPoint::CollectExceptPoint(ArrayPoint &aryCollect,ArrayPoint &aryExcept)
{
	aryExcept.RemoveAll();
	int imax = (int)GetSize();
	for(int i=0;i<imax;i++){ //�������ɐڑ�����
		CPoint &ptCheck = GetAt(i);
		if( aryCollect.Find(ptCheck) < 0 ){ //aryCollect�ȊO�̓_�̂ݒǉ�
			aryExcept.Add(ptCheck);
		}
	}	
}

//�w�萔�ڑ�Point�݂̂�I��
void ArrayPoint::SelectConnectPoint(int nCnum)
{
	int imax = (int)GetSize();
	if( imax > 0 ){
		ArrayPoint aryConnect; //�ڑ��|�C���g�z��
		ArrayPoint aryTmp;
		for(int i=0;i<imax;i++){
			CPoint &pt = GetAt(i);
			if( aryTmp.GetSize() == 0 || aryTmp.IsConnect(pt) ){	//�ڑ��m�F
				aryTmp.Add(pt); 
			}else{ //�ڑ����r�؂ꂽ��
				if( aryTmp.GetSize() >= nCnum ){ //�\���Ȑ��̐ڑ��v�f������Ƃ�����
					aryConnect.Append( aryTmp ); //�V�z��ɒǉ�
				}
				aryTmp.RemoveAll();
			}
		}
		//2����
		CPoint ptStart = GetAt(0);
		if( aryTmp.IsConnect(ptStart) ){
			for(int i=0;i<imax;i++){
				CPoint &pt = GetAt(i);
				if( aryTmp.GetSize() == 0 || (aryTmp.Find(pt)<0 && aryTmp.IsConnect(pt)) ){	//�ڑ��m�F
					aryTmp.Add(pt); 
				}else{ //�ڑ����r�؂ꂽ��
					break;
				}
			}			
		}
		if( aryTmp.GetSize() >= nCnum ){ //�\���Ȑ��̐ڑ��v�f������Ƃ�����
			aryConnect.Append( aryTmp ); //�V�z��ɒǉ�
		}
		this->RemoveAll();
		this->Copy( aryConnect ); //�w�萔�ڑ��z��ƍ����ւ���
	}
}
//�ڑ�Point�ł����H
BOOL ArrayPoint::IsConnect(CPoint &pt) 
{
	for(int i=0;i<GetSize();i++){
		CPoint &pt1 = GetAt(i);
		if( abs(pt1.x-pt.x) <=1 && abs(pt1.y-pt.y)<=1 ){ 
			return TRUE;//�ڑ��|�C���g�����݂���
		}
	}
	return FALSE; //�ڑ��|�C���g�͑��݂��Ȃ�����
}

//�w��x�͈͓��̍��W�������c��
void ArrayPoint::RangeX(double dbXmin,double dbXmax)
{
	int imax = (int)GetSize(); //�������猟�o
	for(int i=imax-1;i>=0;i--){
		int nX = GetAt(i).x;
		if( nX < dbXmin || dbXmax < nX ){ //Range�O�̍��W�����Ɠ_�͍폜
			RemoveAt(i);
		}
	}
}
//�w��y�͈͓��̍��W�������c��
void ArrayPoint::RangeY(double dbYmin,double dbYmax)
{
	int imax = (int)GetSize(); //�������猟�o
	for(int i=imax-1;i>=0;i--){
		int nY = GetAt(i).y;
		if( nY < dbYmin || dbYmax < nY ){ //Range�O�̍��W�����Ɠ_�͍폜
			RemoveAt(i);
		}
	}
}

//���`��
void ArrayPoint::DrawPolyline(CDIB &dib,COLORREF color)
{
	//�������f�o�C�X�R���e�L�X�g�̍쐬
	CDC *pDC = ::AfxGetMainWnd()->GetDC();
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	LOGPEN lp; //�w��F�̃y��
	lp.lopnStyle = PS_SOLID;
	lp.lopnWidth.x = 3;
	lp.lopnColor = color; //�F�w��
	CPen *pPen = g_GlobalMgr.GetPen(lp); //Pen�擾
	CPen *pPenOld = dcMem.SelectObject(pPen);
	CBitmap *pBmpOld = dcMem.SelectObject(dib.GetBitmap());
	int imax = (int)GetSize(); //�������猟�o
	if( imax>0){
		CPoint pt = GetAt(0);
		dcMem.MoveTo(pt);
		for(int i=1;i<imax;i++){
			CPoint pt = GetAt(i);
			dcMem.LineTo(pt);			
		}
	}
	//��n��
	dcMem.SelectObject(pPenOld);
	dcMem.SelectObject(pBmpOld);
	pDC->DeleteDC();
}
//�_�`��
void ArrayPoint::DrawPoints(int nNo)
{
	App *pApp = App::GetInstance();
	CString strNameT;
	int imax = (int)GetCount();
	for (int i = 0; i < imax; i++){
		CPoint &pt = GetAt(i);
		strNameT.Format(L"R%d_%d", nNo, i);
		pApp->DrawRect(strNameT, pt.x - 1, pt.y - 1, pt.x + 1, pt.y + 1, 255, 255, 255, 0, TRUE);
	}
}