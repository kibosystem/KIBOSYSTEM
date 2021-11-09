//Com Port�ʐM�N���X

#include "../stdafx.h"
#include "../KIBO.h"
#include "../KIBOFrame.h"
#include "comport.h"

ComPort::ComPort(void)
{
	Init(); //������
}

ComPort::ComPort(LPCTSTR lpszPortName)
{
	Init(); //������
	Open(lpszPortName,GENERIC_ALL);
}

ComPort::~ComPort(void)
{
	Close();
}

//������
void ComPort::Init()
{
	m_hCom = NULL;
	m_cTerminal = '\n';
	m_nReceiveNum=0; //��M��
}
//Com Port���J��
HANDLE ComPort::Open(LPCTSTR lpszPortName,DWORD dwDesiredAccess/* = GENERIC_READ|GENERIC_WRITE*/)
{
	m_hCom = ::CreateFile(lpszPortName		// �t�@�C����
						,dwDesiredAccess	// �A�N�Z�X���[�h
						,0					// ���L���[�h
						,NULL				// �Z�L�����e�B�L�q�q
						,OPEN_EXISTING		// �쐬���@
						,0					// �t�@�C������
						,NULL);				// �e���v���[�g�t�@�C���̃n���h��

	if( m_hCom == INVALID_HANDLE_VALUE ){
		return NULL;
	}
	return m_hCom;
}

void ComPort::Close()
{
	if( m_hCom != NULL ){
		::CloseHandle(m_hCom);
		m_hCom = NULL;
	}
}

//�ʐM�^�C���A�E�g���Ԑݒ�
BOOL ComPort::SetReadTimeOut(DWORD dwTime)
{
	if( m_hCom != NULL ){
		COMMTIMEOUTS commTimeouts;
		ZeroMemory(&commTimeouts,sizeof(COMMTIMEOUTS));
		commTimeouts.ReadTotalTimeoutConstant = dwTime; //���Ԑ؂�w��
		return ::SetCommTimeouts(m_hCom,&commTimeouts);	
	}
	return FALSE;
}

//COM Port�]�����x�ݒ�
BOOL ComPort::SetCommRate(DWORD dwBaudRate)
{
	if( m_hCom != NULL ){
		DCB dcb;
		GetCommState(m_hCom, &dcb); /* DCB ���擾 */
		dcb.DCBlength = sizeof(DCB);
		dcb.BaudRate = dwBaudRate;
		dcb.fBinary = TRUE;
		dcb.ByteSize = 8;
		dcb.fParity = NOPARITY;		//  �p���e�B�L����ށ@�F�@�p���e�B�Ȃ��̏ꍇ��NOPARITY���w��
									//                                     ��p���e�B�̏ꍇ�� ODDPARITY �@��
		dcb.StopBits = ONESTOPBIT;	// �X�g�b�v�r�b�g�̐��F�@�ʏ�P�r�b�g�� ONESTOPBIT;
		//�n�[�h�E�F�A�t���[����
		dcb.fOutxCtsFlow = FALSE;       // �@CTS�n�[�h�E�F�A�t���[����FCTS������g�p���Ȃ��ꍇ��FLASE���w��
										//   �@�@�@�@�@�@CTS ���������ꍇ��TRUE���w�肵��CTS�M�����Ď����܂��B                     
		dcb.fOutxDsrFlow = FALSE;       //  DSR�n�[�h�E�F�A�t���[����F�g�p���Ȃ��ꍇ��FALSE���w��
		dcb.fDtrControl = DTR_CONTROL_DISABLE;	// DTR�L��/�����F�@�����Ȃ�@DTR_CONTROL_DISABLE;ISABLE
		dcb.fRtsControl = RTS_CONTROL_DISABLE;	// RTS����FRTS��������Ȃ��ꍇ��RTS_CONTROL_DISABLE���w��
												//�@�@�@�@�@RTS���������ꍇ��RTS_CONTROL_ENABLE�@�����w��

		// �\�t�g�E�F�A�t���[����
		dcb.fOutX = FALSE;					// ���M��XON/OFF����̗L���F�@�Ȃ���FLALSE  
		dcb.fInX = FALSE;                   // ��M��XON/XOFF����̗L���F�Ȃ���FALSE  
		dcb.fTXContinueOnXoff = TRUE;		// ��M�o�b�t�@�[���t��XOFF��M��̌p�����M�ہF���M��TRUE
		dcb.XonLim = 512;					// XON��������܂łɊi�[�ł���ŏ��o�C�g���F
		dcb.XoffLim = 512;					// XOFF��������܂łɊi�[�ł���ŏ��o�C�g���F
		dcb.XonChar = 0x11;                 // ���M��XON���� ( ���M�F�r�W�B���� ) �̎w��F
											//�@��ʂɁAXON�����Ƃ���11H ( �f�o�C�X����P�FDC1 )���悭�g���܂�
		dcb.XoffChar = 0x13;				// XOFF�����i���M�s�F�r�W�[�ʍ��j�̎w��F�Ȃ���FALSE
											//�@��ʂɁAXOFF�����Ƃ���13H ( �f�o�C�X����3�FDC3 )���悭�g���܂�
		//���̑�		 
		dcb.fNull = FALSE;          // NULL�o�C�g�̔j���F �j�����遨TRUE
		dcb.fAbortOnError = TRUE;  // �G���[���̓ǂݏ�������I���F�@�I�����遨TRUE
		dcb.fErrorChar = FALSE;    // �p���e�B�G���[�������̃L�����N�^�iErrorChar�j�u���F�@�Ȃ���FLALSE
		dcb.ErrorChar = 0x00;      // �p���e�B�G���[�������̒u���L�����N�^
		dcb.EofChar = 0x03;        // �f�[�^�I���ʒm�L�����N�^�@�F�@��ʂ�0x03(ETX)���悭�g���܂��B
		dcb.EvtChar = 0x02;        // �C�x���g�ʒm�L�����N�^ �F�@��ʂ�0x02(STX)���悭�g���܂�		
		if( ::SetCommState(m_hCom,&dcb) ){
			EscapeCommFunction( GetHandle() , SETRTS | SETDTR );
			return TRUE;
		}
	}
	return FALSE;
}

//������𑗐M���܂��B
DWORD ComPort::SendCommand(LPCTSTR lpszCommand,DWORD dwSleepTime/*=0*/)
{
	CW2A pszA(lpszCommand); //Unicode To Ascii

	DWORD dwWrite = 0;
	WriteFile(m_hCom,pszA,(DWORD)strlen(pszA),&dwWrite,NULL);//����ޑ��M
	if( dwSleepTime > 0 ){
		::Sleep(dwSleepTime); //�R�}���h���s��ҋ@
	}
	return dwWrite;
}
//����R�[�h���M
DWORD ComPort::SendChar(BYTE byCode)
{
	DWORD dwWrite = 0;
	WriteFile(m_hCom,&byCode,1,&dwWrite,NULL);//����ޑ��M
	return dwWrite;	
}
//�I�[�����܂Ŏ�M���܂��B
BOOL ComPort::RecvString(CString &strRecv)
{
	strRecv.Empty();

	//��M
	// �t���[����֌W�̐M���𑗐M�i�t���[����������Ȃ��ꍇ�j
	BOOL isOK = EscapeCommFunction(
					GetHandle() // �@�ʐM�f�o�C�X�̃n���h���FCreateFile()�Ŏ擾�����n���h�����w��
					,SETRTS);	// ��M�\�ł��邱�Ƃ𑊎葤�Ɏ����FRTS���A�N�e�B�u�ɂ��遨SETRTS
								//�i�Q�l�j   RTS���A�N�e�B�u�ɂ��遨CLRRTS�@
	DWORD dwErrors;  /* �G���[��� */
	COMSTAT ComStat; /* �f�o�C�X�̏�� */	
	ClearCommError(m_hCom, &dwErrors, &ComStat);

	char cData = 1;
	DWORD dwRead = 0;
	while(1){ //�I�[�����񂪗���܂œǍ�
		if( !ReadFile(m_hCom,&cData,1,&dwRead ,NULL) ){ 
			//�ǂݍ��ݎ��s
			break; 
		}
		if( cData == 0 || cData == GetTerminal()){ //�I�[�����̏ꍇ
			return TRUE;
		}
		if(cData == 0x09 || cData == 0x0b){
			cData = ' ';
		}
		strRecv += cData;
		cData = 0;
	}
	return FALSE;
}

//��M�҂��X���b�h
static UINT WaitThreadProc( LPVOID pParam )
{
	ComPort *pComPort = (ComPort *)pParam;

	CString strRecv;
	//��M
	// �t���[����֌W�̐M���𑗐M�i�t���[����������Ȃ��ꍇ�j
	BOOL isOK = EscapeCommFunction(
					pComPort->GetHandle() // �@�ʐM�f�o�C�X�̃n���h���FCreateFile()�Ŏ擾�����n���h�����w��
					,SETRTS);	// ��M�\�ł��邱�Ƃ𑊎葤�Ɏ����FRTS���A�N�e�B�u�ɂ��遨SETRTS
								//�i�Q�l�j   RTS���A�N�e�B�u�ɂ��遨CLRRTS�@
	DWORD dwErrors;  /* �G���[��� */
	COMSTAT ComStat; /* �f�o�C�X�̏�� */	
	ClearCommError(pComPort->GetHandle(), &dwErrors, &ComStat);

	char cData = 1;
	DWORD dwRead = 0;
	while(IsWindowOK()){ //�I�[�����񂪗���܂œǍ�
		if( !ReadFile(pComPort->GetHandle(),&cData,1,&dwRead ,NULL) ){ 
			//�ǂݍ��ݎ��s
			break; 
		}
		if( cData != 0 ){ //�I�[�����̏ꍇ
			strRecv += cData;
		}
		if( strRecv.GetLength() >= pComPort->GetReceiveNum()){
			break;
		}
		cData = 0;
	}
	pComPort->SetRecvString(strRecv); //��M������ݒ�
	pComPort->SetEnd(true); //�I��
	return 0;
}
//�w�蕶������M���܂��B
void ComPort::RecvString(int nNum)
{
	SetEnd(false);
	SetReceiveNum(nNum); //��M�������o�^
	::AfxBeginThread(WaitThreadProc,this); //�X���b�h���s
}

