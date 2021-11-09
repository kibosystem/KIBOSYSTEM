#include "../stdafx.h"
#include "VisualObject.h"
#include "Object.h"
#include "../KIBOView.h"

//�R���X�g���N�^
VisualObject::VisualObject(void)
{
	m_isVisible = true; //�\��ON
	m_nX = 0;	//x���W
	m_nY = 0;	//y���W
	m_byType = TYPE_NONE; //�w��Ȃ�
}

//�f�X�g���N�^
VisualObject::~VisualObject(void)
{
}

//�R���X�g���N�^
AryVO::AryVO(void)
{
}
//�f�X�g���N�^
AryVO::~AryVO(void)
{
}
int AryVO::Find(VisualObject *pVO)
{
	for(int i=0,imax=(int)GetSize();i<imax;i++){
		if( pVO == GetAt(i) ){ //��������
			return i;
		}
	}
	//������Ȃ�����
	return -1;
}

//�R���X�g���N�^
VisualObjectMgr::VisualObjectMgr()
{
	m_isVisible = true; //Default�\��
}
//�f�X�g���N�^
VisualObjectMgr::~VisualObjectMgr()
{
	//�L���z��
	for(int i=0,imax=(int)m_aryValid.GetSize();i<imax;i++){
		//TRACE(L"Delete:%s\n",m_aryValid[i]->GetName());
		delete m_aryValid[i]; 
	}
	//�����z��
	for(int i=0,imax=(int)m_aryInvalid.GetSize();i<imax;i++){
		//TRACE(L"Delete:%s\n",m_aryInvalid[i]->GetName());
		delete m_aryInvalid[i]; 
	}

	m_aryValid.RemoveAll();	
	m_aryInvalid.RemoveAll();
}

//Object�̍쐬
VisualObject *VisualObjectMgr::CreateVisualObject(int nType,CString &strName)
{
	VisualObject *pVO = NULL;

	//���łɑ��݂���ꍇ
	for(int i=0;i<m_aryValid.GetSize();i++){
		VisualObject *pCheck = m_aryValid[i];
		if( pCheck->GetName() == strName ){ //����
			m_aryValid.RemoveAt(i); //�L���z�񂩂�O��
			pVO = pCheck;
			break;
		}
	}
	if( pVO == NULL ){
		//�����z�񂩂�擾	
		for(int i=0,imax=(int)m_aryInvalid.GetSize();i<imax;i++){
			VisualObject *pCheck = m_aryInvalid[i];
			if( pCheck->GetType() == nType ){ //��������
				m_aryInvalid.RemoveAt(i); //�����z�񂩂�O��
				pVO = pCheck; //�߂�l
				break;
			}
		}
	}
	//�V�K�ɍ쐬
	if( pVO == NULL ){
		switch(nType){
			case VisualObject::TYPE_BITMAP://Bitmap
				break;
			case VisualObject::TYPE_LINE://��
				{
					LineObject *pLO = new LineObject();
					pVO = (VisualObject *)pLO;
				}
				break;
			case VisualObject::TYPE_RECT://��`
				{
					RectObject *pRO = new RectObject();
					pVO = (VisualObject *)pRO;
				}
				break;
			case VisualObject::TYPE_CIRCLE://�~
				{
					CircleObject *pCO = new CircleObject();
					pVO = (VisualObject *)pCO;
				}
				break;
			case VisualObject::TYPE_STRING://������
				{
					StringObject *pSO = new StringObject();
					pVO = (VisualObject *)pSO;
				}
				break;
			case VisualObject::TYPE_POLYLINE://�|���S���g��
				{
					PolylineObject *pPO = new PolylineObject();
					pVO = (VisualObject *)pPO;
				}
				break;
		}
		//TRACE(L"Create:%s\n",strName);
	}
	if( pVO != NULL ){
		pVO->SetType(nType);
		pVO->SetName(strName);
		m_aryValid.Add(pVO); //�L���z��֓o�^
	}
	return pVO;
}

//�w��VisualObject���őO�ʂɈړ�
void VisualObjectMgr::SetFront(VisualObject *pVO)
{
	//�L���z��
	for(int i=0,imax=(int)m_aryValid.GetSize();i<imax;i++){
		if( m_aryValid[i] == pVO ){
			m_aryValid.RemoveAt(i); //�L���z�񂩂�폜
			m_aryValid.Add(pVO); //�őO�ʂɒǉ�
			break ;
		}
	}	
}

//�w��VisualObject�𖳌���
void VisualObjectMgr::Remove(VisualObject *pVO) 
{
	//�L���z��
	for(int i=0,imax=(int)m_aryValid.GetSize();i<imax;i++){
		if( m_aryValid[i] == pVO ){
			m_aryValid.RemoveAt(i); //�L���z�񂩂�
			m_aryInvalid.Add(pVO);	//�����z��ֈړ�
			break ;
		}
	}	
}
//�SVisualObject�𖳌���
void VisualObjectMgr::RemoveAll()
{
	//�L���z�� -> �����z��
	while(m_aryValid.GetSize() > 0){ //�L���z�񂪑��݂����
		VisualObject *pVO = m_aryValid[0]; //�擪�v�f���擾
		m_aryValid.RemoveAt(0); //�L���z�񂩂�擪�v�f���O��
		m_aryInvalid.Add(pVO);	//�����z��ֈړ�
	}
}
//�w�薼�̂��܂ރI�u�W�F�N�g�𖳌������܂�
void VisualObjectMgr::RemoveByName(const CString &strName) 
{
	//�L���z��
	for(int i=(int)m_aryValid.GetSize()-1;i>=0;i--){
		VisualObject *pVO = m_aryValid[i];
		if( pVO->GetName().Find(strName) >= 0 ){ //���O���܂�ł���ꍇ
			m_aryValid.RemoveAt(i); //�L���z�񂩂�
			m_aryInvalid.Add(pVO);	//�����z��ֈړ�
		}
	}	
}
//�w��I�u�W�F�N�g�ȊO���폜
void VisualObjectMgr::RemoveExcept(const CString &strName) 
{
	//�L���z��
	for(int i=(int)m_aryValid.GetSize()-1;i>=0;i--){
		VisualObject *pVO = m_aryValid[i];
		if( pVO->GetName().Find(strName) < 0 ){ //���O���܂�łȂ��ꍇ�̂�
			m_aryValid.RemoveAt(i); //�L���z�񂩂�
			m_aryInvalid.Add(pVO);	//�����z��ֈړ�
		}
	}
}

//�w��Visual Object�擾
VisualObject* VisualObjectMgr::GetVO(CString strName)
{
	//�L���z��
	for(int i=(int)m_aryValid.GetSize()-1;i>=0;i--){
		VisualObject *pVO = m_aryValid[i];
		if( strName == pVO->GetName() ){ //��������
			return pVO;
		}
	}
	//������Ȃ�����
	return NULL;
}

//Visual Object�̕`��
void VisualObjectMgr::Draw(CDC *pDC)
{
	if( m_isVisible ){ //Visual Object�S�̂̕\���E��\���؂�ւ�
		for(int i=0,imax=(int)m_aryValid.GetSize();i<imax;i++){
			if( m_aryValid[i]->IsVisible() ){
				m_aryValid[i]->Draw(pDC); 
			}
		}
		//TRACE(L"imax:%d\n",m_aryValid.GetSize());
	}
}
//���C���ǉ�
void VisualObjectMgr::CreateLineObject(CString strName,const CLine &line,Color color)
{
	//CKIBOView *pView = GetView();
	//double dbRateCx = pView->GetRateCx();
	//double dbRateCy = pView->GetRateCy();
	LineObject *pLO = (LineObject *)CreateVisualObject(VisualObject::TYPE_LINE,strName);
	pLO->SetPointS(line.m_ptS.x,line.m_ptS.y);
	pLO->SetPointE(line.m_ptE.x,line.m_ptE.y);
	pLO->SetColor(color);
	pLO->SetVisible(true);
}
//��`�ǉ�
void VisualObjectMgr::CreateRectObject(CString strName,const CRect &rect,Color color,BOOL isPaint)
{
	CKIBOView *pView = GetView();
	double dbRateCx = pView->GetRateCx();
	double dbRateCy = pView->GetRateCy();
	CRect rc((int)(rect.left   * dbRateCx)
			,(int)(rect.top    * dbRateCy)
			,(int)(rect.right  * dbRateCx)
			,(int)(rect.bottom * dbRateCy));

	RectObject *pRO = (RectObject *)CreateVisualObject(VisualObject::TYPE_RECT,strName);
	pRO->SetRect(rc);
	pRO->SetColor(color);
	pRO->SetPaint(isPaint ? true : false);
	pRO->SetVisible(true);
}

//�~�ǉ�
void VisualObjectMgr::CreateCircleObject(CString strName,const CRect &rect,Color color,BOOL isPaint)
{
	CKIBOView *pView = GetView();
	double dbRateCx = pView->GetRateCx();
	double dbRateCy = pView->GetRateCy();
	CRect rc((int)(rect.left   * dbRateCx)
			,(int)(rect.top    * dbRateCy)
			,(int)(rect.right  * dbRateCx)
			,(int)(rect.bottom * dbRateCy));
	CircleObject *pCO = (CircleObject *)CreateVisualObject(VisualObject::TYPE_CIRCLE,strName);
	pCO->SetRect(rc);
	pCO->SetColor(color);
	pCO->SetPaint(isPaint ? true : false);	
	pCO->SetVisible(true);
}

//�|���S���g���ǉ�
void VisualObjectMgr::CreatePolylineObject(CString strName,ArrayPoint &ptAry,COLORREF color,bool isClosed /*=true*/)
{
	ArrayPoint ary;
	ary.Copy( ptAry );
	if( ary.GetSize() > 0 ){
		CKIBOView *pView = GetView();
		double dbRateCx = pView->GetRateCx();
		double dbRateCy = pView->GetRateCy();

		PolylineObject *pPO = (PolylineObject *)CreateVisualObject(VisualObject::TYPE_POLYLINE,strName);
		for(int i=0,imax=(int)ary.GetSize();i<imax;i++){
			if(ptAry[i].x < 0){
				break;
			}
			ary[i].x = (int)(ary[i].x * dbRateCx);
			ary[i].y = (int)(ary[i].y * dbRateCy);
		}
		pPO->SetPoints(ary);
		pPO->SetColor(color);
		pPO->SetClosed(isClosed);
		pPO->SetVisible(true);
	}
}
//�|���S���g���ǉ�
void VisualObjectMgr::CreatePolylineObject(CString strName,const LPPOINT lpPoints,int nCnt,COLORREF color,bool isClosed /*=true*/)
{
	if( nCnt > 0 ){
		CKIBOView *pView = GetView();
		double dbRateCx = pView->GetRateCx();
		double dbRateCy = pView->GetRateCy();

		PolylineObject *pPO = (PolylineObject *)CreateVisualObject(VisualObject::TYPE_POLYLINE,strName);
		ArrayPoint ptAry;
		CPoint point;
		for(int i=0;i<nCnt;i++){
			point.x = (int)(lpPoints[i].x * dbRateCx + 0.5);
			point.y = (int)(lpPoints[i].y * dbRateCy + 0.5);
			ptAry.Add(point);
		}
		pPO->SetPoints(ptAry);
		pPO->SetColor(color);
		pPO->SetClosed(isClosed);
		pPO->SetVisible(true);
	}
}

//������ǉ�
void VisualObjectMgr::CreateStringObject(CString strName,int nX,int nY,CString strText,COLORREF color,LOGFONT &lf)
{
	StringObject *pSO = (StringObject *)CreateVisualObject(VisualObject::TYPE_STRING,strName);
	CKIBOView *pView = GetView();
	double dbRateCx = pView->GetRateCx();
	double dbRateCy = pView->GetRateCy();

	HFONT hFont = g_GlobalMgr.GetFontHandle(lf);
	nX = (int)(nX * dbRateCx + 0.5);
	nY = (int)(nY * dbRateCy + 0.5);
	pSO->SetFont(hFont);
	pSO->SetX(nX);
	pSO->SetY(nY);
	pSO->SetText(strText);
	pSO->SetColor(color);
	pSO->InitRect();
	pSO->SetVisible(true);
}