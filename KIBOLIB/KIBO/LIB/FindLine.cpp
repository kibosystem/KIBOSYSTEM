#include "../stdafx.h"
#include "FindLine.h"
#include "../LIB/color.h"

//FindLine::FindLine(void)
//{
//	ZeroMemory(m_sn,sizeof(m_sn));	//�O�p�֐��e�[�u���i�T�C���j	
//	ZeroMemory(m_cs,sizeof(m_cs));	//�O�p�֐��e�[�u���i�R�T�C���j	
//}
//
//FindLine::~FindLine(void)
//{
//}
//
////������
//BOOL FindLine::Init(CString strPath)
//{
//	CDIB dib;
//	if( m_dib.Load(strPath) ){
//		m_YMAX = m_dib.GetHeight();
//		m_XMAX = m_dib.GetWidth();
//	
//		//�O�p�֐��e�[�u�����쐬
//		double PIK = PI/THETA_MAX;
//		for(int i=0;i<THETA_MAX;i++){
//			m_sn[i] = (float)sin(PIK*i);
//			m_cs[i] = (float)cos(PIK*i);
//		}
//		return TRUE;
//	}
//	return FALSE;
//}
////������
//BOOL FindLine::Init(CDIB &dib)
//{
//	dib.Copy( m_dib );
//	m_YMAX = m_dib.GetHeight();
//	m_XMAX = m_dib.GetWidth();
//	
//	//�O�p�֐��e�[�u�����쐬
//	double PIK = PI/THETA_MAX;
//	for(int i=0;i<THETA_MAX;i++){
//		m_sn[i] = (float)sin(PIK*i);
//		m_cs[i] = (float)cos(PIK*i);
//	}
//	return TRUE;
//}
//
////�J�E���^�[�f�[�^
//struct CountData
//{
//	int m_nCnt; //�J�E���^�[
//};
//
//
////�������o
//BOOL FindLine::Find(BYTE byLine,int nPixels,int nCount,int nCheckLen)
//{
//	int end_flag=0;   //�J��Ԃ����I��������t���O
//	int count=0;      //���o���ꂽ�����܂��͉~�̌��J�E���^
//	int x,y;
//	// ---------------------- Hough�ϊ� --------------------------
//	//�����̏ꍇ -------------------------------------------------	
//	static CountData counter[THETA_MAX][2*RHO_MAX];//�������o�p�p�x�J�E���^
//	ZeroMemory( counter,sizeof(counter) );
//	for(y=0;y<m_YMAX;y++){
//		for(x=0;x<m_XMAX;x++){
//			BYTE *pBits = m_dib.GetPBitsB(x,y);
//			if( *pBits == byLine && *(pBits+1) == byLine ){ //���C���F(RGB����l�j
//				for(int theta=0;theta<THETA_MAX;theta++){
//					int rho=(int)(x*m_cs[theta]+y*m_sn[theta]+0.5); //x,y -> �ƃϕϊ�
//					CountData *pCData = &counter[theta][rho+RHO_MAX];
//					pCData->m_nCnt++; //�J�E���^�[
//				}
//				//�F�h�肵�܂�
//				*pBits = 255;
//			}
//		}
//	}
//
//	// --------------------- Hough�t�ϊ� -------------------------
//	//�����̏ꍇ -------------------------------------------------
//	int counter_max;
//	int theta_max = 0;
//	int rho_max = -RHO_MAX;
//	COutputView *pOutput = GetOutputView();
//	while(end_flag == 0 && count < nCount){ //������nPixels�s�N�Z���ȉ����A������5�{���o���ꂽ��I��
//		count++;
//		counter_max=0;         
//		//counter���ő�ɂȂ�theta_max��rho_max�����߂�
//		for(int theta=0;theta<THETA_MAX;theta++){//�S�ƃ��[�v
//			for(int rho=-RHO_MAX;rho<RHO_MAX;rho++){//�S�σ��[�v
//				CountData *pCData = &counter[theta][rho+RHO_MAX];
//				if(pCData->m_nCnt > counter_max){
//					counter_max = pCData->m_nCnt;//�ő�l�̍����ւ�
//					//nPixels�s�N�Z���ȉ��̒����ɂȂ�Ό��o���I��
//					if(counter_max<=nPixels){ end_flag=1;}
//					else{end_flag=0;}
//					theta_max = theta;
//					rho_max   = rho;
//				}
//			}
//		}
//		//rho_max��theta_max�̕`��
//		double dbX = rho_max / m_cs[theta_max]; //y==0
//		double dbY = rho_max / m_sn[theta_max]; //x==0
//		// (dbX,0) - (0,dbY)  ��ʂ钼���Ɓ@��`(0,0,nW,nH)�Ƃ̌�_�����߂�
//		// t *  (dbX,-dbY) + (0,dbY)
//
//		PointDbl pt[4];
//		//x==0�Ƃ̌�_�@(0,dbY)
//		pt[0].x = 0;
//		pt[0].y = dbY;			
//		//x==m_XMAX�Ƃ̌�_ t = m_XMAX / dbX
//		pt[1].x = m_XMAX;
//		pt[1].y = (dbX != 0) ? (dbY - m_XMAX * dbY / dbX) : -1;
//		//y==0�Ƃ̌�_ (dbX,0)
//		pt[2].x = dbX;
//		pt[2].y = 0;
//		//y==m_YMAX�Ƃ̌�_ t = (dbY-m_YMAX)/dbY
//		pt[3].x = (dbY != 0) ? (dbX*(dbY-m_YMAX) / dbY) : -1;
//		pt[3].y = m_YMAX;
//
//		//��`�ƒ����Ƃ̌�_���Q�ɍi��
//		PointDbl pt1(0,0);
//		PointDbl pt2(0,0);
//		for(int i=0;i<4;i++){
//			if( 0 <= pt[i].x && pt[i].x <= m_XMAX  && 0 <= pt[i].y && pt[i].y <= m_YMAX ){
//				if(pt1.x <= 0 && pt1.y <= 0){
//					pt1 = pt[i];
//				}else{
//					pt2 = pt[i];
//					break;
//				}
//			}
//		}
//		//Start:pt1 End:pt2 
//		DrawLine(pt1,pt2,byLine,nPixels,nCheckLen); //CHECK_LEN���Ƃɒ������o����
//
//		PointDbl vec = pt2 - pt1; //�����x�N�g��
//		double dbLen = vec.GetLength();
//		vec.x /= dbLen; //�P�ʃx�N�g����
//		vec.y /= dbLen;
//
//		pt1.x = (int)(nCheckLen/2 * vec.x + pt1.x + 0.5); //�����̋����Â炷
//		pt1.y = (int)(nCheckLen/2 * vec.y + pt1.y + 0.5);
//		DrawLine(pt1,pt2,byLine,nPixels,nCheckLen); //CHECK_LEN���Ƃɒ������o����
//
//		//�ߖT�̒���������
//		int nImin = -THETA_MAX / 2;
//		int nImax = THETA_MAX / 2;
//		for(int j=-10;j<=10;j++){
//			for(int i=nImin;i<=nImax;i++){
//				if(theta_max+i<0){//�}�C�i�X�p�x
//					theta_max+=THETA_MAX;
//					rho_max=-rho_max;
//				}
//				if(theta_max+i>=THETA_MAX){
//					theta_max-=THETA_MAX;
//					rho_max=-rho_max;
//				}
//				if(rho_max+j<-RHO_MAX || RHO_MAX <= rho_max+j){
//					continue;
//				}
//				counter[theta_max+i][rho_max+RHO_MAX+j].m_nCnt = 0;
//			}
//		}
//	}
//	return TRUE;
//}
//
////CHECK_LEN���Ƃɒ������o����
//void FindLine::DrawLine(PointDbl pt1,PointDbl pt2,BYTE byLine,int nPixels,int nCheckLen)
//{
//	//pt1 - pt2�Ԃ̋���
//	PointDbl vec = pt2 - pt1; //�����x�N�g��
//	double dbLen = vec.GetLength();
//	vec.x /= dbLen; //�P�ʃx�N�g����
//	vec.y /= dbLen;
//	int tmax = (int)dbLen;
//
//	//(x,y) = t * vec + pt1
//	CPoint ptS((int)pt1.x,(int)pt1.y); //���C���[�_
//	CPoint ptE(0,0); //���C���[�_
//	int nCnt = 0;
//	int nCheckPoint = 0;
//	int nArea = 0; //�����G���A�ԍ�
//	for(int t=0;t<tmax;t++){
//		int nX = min(m_XMAX-1,(int)(t * vec.x + pt1.x + 0.5));
//		int nY = min(m_YMAX-1,(int)(t * vec.y + pt1.y + 0.5));
//		nArea = t / nCheckLen; //���݂̃G���A�ԍ�
//		if( nArea > nCheckPoint ){ //���̃G���A�Ɉڂ�����
//			ptE.x = nX;
//			ptE.y = nY;
//			nCheckPoint = nArea; //�V�����G���A�Ɉړ�
//			if( nCnt >= nPixels ){ //�G���[���C��
//				m_dib.Line(ptS.x,ptS.y,ptE.x,ptE.y,COLOR_RED);
//			}else{
//				m_dib.Line(ptS.x,ptS.y,ptE.x,ptE.y,COLOR_GREEN);
//			}
//			ptS = ptE; //���̊J�n�_��
//			nCnt = 0; //�N���A�|�C���g
//		}							
//		//�J�E���g
//		BYTE byB = m_dib.GetBitsB(nX,nY);
//		BYTE byG = m_dib.GetBitsG(nX,nY);
//		BYTE byR = m_dib.GetBitsR(nX,nY);
//		if( byB == 255 && byG <= 0 ){//&& byR <= 0){ 
//			nCnt++; //�J�E���g
//		}
//	}
//}
//
//void FindLine::Draw(CDC &dc)
//{
//	//m_dib.Draw(&dc,0,0,m_dib.GetWidth(),m_dib.GetHeight(),0,0);
//}