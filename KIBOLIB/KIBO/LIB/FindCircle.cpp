#include "../stdafx.h"
#include "FindCircle.h"
//
//FindCircle::FindCircle()
//{
//	m_nRmin = 0;	//���a�ŏ�
//	m_nRmax = 0;	//���a�ő�
//	m_nPixelsMin = 0; //�w���f����
//	m_nPixelsMax = 0; //�w���f����
//	ZeroMemory(m_sn,sizeof(m_sn));	//�O�p�֐��e�[�u���i�T�C���j	
//	ZeroMemory(m_cs,sizeof(m_cs));	//�O�p�֐��e�[�u���i�R�T�C���j
//}
//FindCircle::~FindCircle()
//{
//
//}
//BOOL FindCircle::Init(CDIB &dib,int nRmin,int nRmax,int nPixelsMin,int nPixelsMax)
//{
//	m_aryCircle.clear();
//	//�摜�t�@�C����ǂݍ���
//
//	int nCx = 640;
//	int nCy = 480;
//	CDC dcMem;
//	dcMem.CreateCompatibleDC(NULL);
//	CDIB dibSrc;
//	if( dibSrc.Create(nCx,nCy) ){
//		CBitmap *pOld = dcMem.SelectObject( dibSrc.GetBitmap() );	
//		dib.StretchDraw(&dcMem,0,0,nCx,nCy);
//		dcMem.SelectObject( pOld );
//
//		dibSrc.WriteBitmap(L"F:\\tmp\\tmp.bmp");
//	}
//
//	if( m_dibGray.Copy(dibSrc) ){ //�t���J���[ �� Gray
//		int nW = m_dibGray.GetWidth();
//		int nH = m_dibGray.GetHeight();
//		if( nW > 0 && nH > 0 ){//�ω���������������
//			if( m_nRmax != nRmax || m_nRmin != nRmin || m_nPixelsMin != nPixelsMin || m_nPixelsMax != nPixelsMax){
//				m_nPixelsMin = nPixelsMin; //����f��
//				m_nPixelsMax = nPixelsMax; //����f��
//				m_nRmin = nRmin; //���a
//				m_nRmax = nRmax;
//				//�O�p�֐��e�[�u�����쐬
//				double PIK = PI/THETA_MAX_C;
//				for(int i=0;i<THETA_MAX_C;i++){
//					m_sn[i] = (float)sin(PIK*i);
//					m_cs[i] = (float)cos(PIK*i);
//				}
//				//�ΐ����e�[�u�����쐬
//				m_diagonal.resize(nW*nH);
//				for(int y=0;y<nH;y++){
//					for(int x=0;x<nW;x++){
//						m_diagonal[y*nW+x]=(short)(sqrt((float)(y*y+x*x))+0.5);
//					}
//				}
//			}
//		}
//		return TRUE;
//	}
//	return FALSE;
//}
//
////�n�t�ϊ��~���o
//void FindCircle::Find()
//{
//	// ---------------------- Hough�ϊ� --------------------------
//	int x,y;
//	int centerX,centerY,radius;
//    //�~���o�p�p�x�J�E���^
//	vector<short> v_counterMax; //Max�l
//	vector<short> v_counterMin; //Min�l
//
//	BYTE *pGrayT = m_dibGray.GetBits();
//	int nW = m_dibGray.GetWidth();
//	int nH = m_dibGray.GetHeight();
//	int nXRmax = nW * m_nRmax;
//	v_counterMax.resize(nH*nXRmax);
//	v_counterMin.resize(nH*nXRmax);
//	int nLineSize = m_dibGray.GetLineSize();	
//	for(y=0;y<nH;y++){
//		BYTE *pGray = pGrayT + (nH-1-y)*nLineSize;
//		for(x=0;x<nW;x++){
//			BYTE byVal = *pGray++;
//			//if( byVal == 255){ //���C���lMax
//			//	//x,y��ʉ߂���S�Ẳ~�e�[�u�����C���N�������g
//			//	for(centerY=0;centerY<nH;centerY++){
//			//		int distY=abs(y-centerY);
//			//		if(distY<m_nRmax){
//			//			for(centerX=0;centerX<nW;centerX++){
//			//				int distX = abs(x-centerX);
//			//				radius = m_diagonal[distY*nW+distX];
//			//				if(radius<m_nRmax){
//			//					v_counterMax[centerY * nXRmax + centerX * m_nRmax + radius] += 1;
//			//				}							
//			//			}
//			//		}
//			//	}
//   //         }
//			if( byVal == 0 ){ //���C���lMin
//				//x,y��ʉ߂���S�Ẳ~�e�[�u�����C���N�������g
//				for(centerY=0;centerY<nH;centerY++){
//					int distY=abs(y-centerY);
//					if(distY<m_nRmax){
//						for(centerX=0;centerX<nW;centerX++){
//							int distX = abs(x-centerX);
//							radius = m_diagonal[distY*nW+distX];
//							if(radius<m_nRmax){
//								v_counterMin[centerY * nXRmax + centerX * m_nRmax + radius] += 1;
//							}
//						}
//					}
//				}
//            }
//		}
//	}
//
//	// --------------------- Hough�t�ϊ� -------------------------
//	CIRCLE result; //���o����
//	int end_flag=0;   //�J��Ԃ����I��������t���O
//	int count=0;      //���o���ꂽ�����܂��͉~�̌��J�E���^
//	int counter_max;
//	while(count<5){
//		count++;
//		counter_max = 0;
//        //counter���ő�ɂȂ�centerX_max�A
//		//centerY_max��radius_max�����߂�
//		for(centerY=0;centerY<nH;centerY++){
//			for(centerX=0;centerX<nW;centerX++){
//				for(radius=m_nRmin;radius<m_nRmax;radius++){
//					int nIndex = centerY*nXRmax+centerX*m_nRmax+radius;
//					int nPixMin = v_counterMin[nIndex]; //0  �̉�f��
//					int nPixMax = v_counterMax[nIndex]; //255�̉�f��
//					if(m_nPixelsMin <= nPixMin && m_nPixelsMax <= nPixMax){//�ǂ������𒴂���K�v����
//						int nCnt = nPixMin + nPixMax;
//						if(nCnt > counter_max){ //�~���o�p�p�x�J�E���^�ő�l�����߂�
//							counter_max = nCnt;
//							result.x=centerX;
//							result.y=centerY;
//							result.r=radius;
//							result.val0 = nPixMin;
//							result.val255 = nPixMax;
//						}
//					}
//				}
//			}
//		}
//		if(result.val0 < m_nPixelsMin && result.val255 < m_nPixelsMax ){//�w���f����艺�̉~�ɂȂ�Ό��o���I��
//			break;
//		}
//		//�ߖT�̉~������
//		for(int k=-10;k<=10;k++){
//			if(result.y+k>=nH || result.y+k<0){ continue;}
//			for(int j=-10;j<=10;j++){
//				if(result.x+j>=nW || result.x+j<0){ continue;}
//				for(int i=-20;i<=20;i++){
//					int nR = result.r+i;
//					if( 0 < nR && nR <= m_nRmax ){
//						int nIndex = (result.y+k)*nXRmax+(result.x+j)*m_nRmax+nR;
//						v_counterMax[nIndex]=0;
//						v_counterMin[nIndex]=0;
//					}
//			   }
//			}
//		}          
//		//���o�����~
//		if( counter_max > 0 ){
//			m_aryCircle.push_back(result);
//		}
//	}
//}
////���ʏo��
//void FindCircle::Output()
//{
//	COutputView *pOutput = GetOutputView();
//	pOutput->Clear();
//	for(int i=0,imax=(int)m_aryCircle.size();i<imax;i++){
//		CIRCLE &result = m_aryCircle[i];
//		pOutput->Output(true,L"���W:(%d,%d) ���a:%d ��:%d ��:%d",result.x,result.y,result.r,result.val0,result.val255);
//	}
//}
