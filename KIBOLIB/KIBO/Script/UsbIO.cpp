#include "../stdafx.h"
#include "../KIBO.h"
#include "../KIBOFrame.h"
#include "../enum.h"
#include "UsbIO.h"
#include "App.h"

#ifdef USE_USBIO 

//���^USBIO
//#define VENDOR_ID				0x1352
//#define PRODUCT_ID			0x0100

//�e�N�m�L�b�g��USBIO
#define VENDOR_ID				0x12ED
#define PRODUCT_ID				0x1003
#define MAX_ENUM_INDEX			100

HANDLE OpenUsbIo();
BYTE InputUsbIo(HANDLE hHid, int iPort);
BOOL OutputUsbIo(HANDLE hHid,int iPort, int iData);

//USBIO���͑҂��^�C�}�[
static void CALLBACK EXPORT TimerProc(HWND hWnd,UINT nMsg,UINT_PTR unID,DWORD dwTime)
{
	UsbIO *pUsbIO = UsbIO::GetInstance();
	if(pUsbIO == NULL){
		return ;
	}
	CArray<IOScript> &aryIO = pUsbIO->m_aryIOScript;
	for(int i=0;i<aryIO.GetSize();i++){
		IOScript& io = aryIO[i];
		int nRet = pUsbIO->Read(io.m_nPort,io.m_nNo);
		if( nRet == io.m_nState ){ //���͂���v����
			//�X�N���v�g���s
			//GetFrame()->ScriptExec(io.m_strFileName);
		}
	}
	//���ꏈ��///////////////////////////////////////////
	App *pApp = App::GetInstance();
	if( pUsbIO->Read(1,2) == 0){ //IN_RESET=2;  //USBIO���͂ɂ�郊�Z�b�g
		pApp->SetNum(L"NGFLAG",0);
	}else if( pUsbIO->Read(1,3) == 0 ){//��STOP
		pApp->SetNum(L"STOPFLAG",1);
	}
}

static CommandTable s_table[] =
{
	 {UsbIO::FUNC_READ	,2,L"Read"}	//�ǂݍ���
	,{UsbIO::FUNC_WRITE	,3,L"Write"}//�o��
	,{UsbIO::FUNC_INIT	,2,L"Init"}	//USBIO�̏�����
	,{UsbIO::FUNC_WAIT	,3,L"Wait"}	//USBIO��ԕω��҂�
	,{UsbIO::FUNC_SETTIMER ,4,L"SetTimer"}//USBIO�^�C�}�[
	,{UsbIO::FUNC_STARTTIMER,0,L"StartTimer"} //USBIO�^�C�}�[�J�n
	,{UsbIO::FUNC_KILLTIMER,0,L"KillTimer"}//USBIO�^�C�}�[
	,{UsbIO::FUNC_MAX	,0,NULL}	//�R�}���h��
};

static UsbIO *s_pInstance = NULL; //�B��̃N���X����
UsbIO* UsbIO::GetInstance()//���̎擾
{
	return s_pInstance;
}

UsbIO::UsbIO(void)
{
	s_pInstance = this;
	m_hHid = NULL;
}

UsbIO::~UsbIO(void)
{
	if( m_hHid != NULL ){
		CloseHandle(m_hHid); 
		m_hHid = NULL;
	}
}

//�֐����`�F�b�N
STDMETHODIMP UsbIO::GetIDsOfNames(REFIID riid,LPOLESTR __RPC_FAR *rgszNames
						,UINT cNames ,LCID lcid
						,DISPID __RPC_FAR *rgDispId)
{
	int nID = GetIDFromName(*rgszNames,s_table);
	if( nID > 0 ){
		*rgDispId = nID;
		return S_OK;
	}
	return DISPID_UNKNOWN;
}

//���s
STDMETHODIMP UsbIO::Invoke(DISPID dispIdMember	//Function ID �ԍ�
					,REFIID riid
					,LCID lcid
					,WORD wFlags
					,DISPPARAMS FAR *pDispParams //Script������
					,VARIANT FAR *pVarResult
					,EXCEPINFO FAR *pExcepInfo
					,unsigned int FAR *puArgErr)
{
	CommandTable *pCmd = GetTableFromID(s_table,dispIdMember);
	if(pCmd == NULL || !IsArgOK(pDispParams,pCmd) ){ //�����`�F�b�N
		return S_FALSE;
	}		

	switch(dispIdMember){
		case FUNC_READ:	//�ǂݍ���
			{
				int nPort = GetValientInt(&pDispParams->rgvarg[1]); //Port �ԍ�
				int nNo = GetValientInt(&pDispParams->rgvarg[0]);	//���͔ԍ�
				int nRet = Read(nPort,nNo); //�w��ԍ��ǂݍ���
				ReturnInt(wFlags,pVarResult,nRet); //Int
			}
			break;
		case FUNC_WRITE://�o��
			{
				int nPort  = GetValientInt(&pDispParams->rgvarg[2]); //Port �ԍ�
				int nNo	   = GetValientInt(&pDispParams->rgvarg[1]); //���͔ԍ�
				int nState = GetValientInt(&pDispParams->rgvarg[0]); //ON or OFF
				Write(nPort,nNo,nState); //�w��o�͔ԍ��ɏ�������
			}
			break;
		case FUNC_INIT:	//USBIO�̏�����
			{
				BYTE byData1 = (BYTE)pDispParams->rgvarg[1].ulVal; //�f�[�^�P
				BYTE byData2 = (BYTE)pDispParams->rgvarg[0].ulVal; //�f�[�^�Q
				BOOL isOK = Init(byData1,byData2);
				ReturnBool(wFlags,pVarResult,isOK);
			}
			break;
		case FUNC_WAIT:	//USBIO��ԕω��҂�
			{
				int nPort  = GetValientInt(&pDispParams->rgvarg[2]); //Port �ԍ�
				int nNo	   = GetValientInt(&pDispParams->rgvarg[1]); //���͔ԍ�
				int nState = GetValientInt(&pDispParams->rgvarg[0]); //ON or OFF
				int nStatus = Wait(nPort,nNo,nState); //��ԑ҂�
				ReturnInt(wFlags,pVarResult,nStatus);
			}
			break;
		case FUNC_SETTIMER: //USBIO�^�C�}�[�J�n
			{
				IOScript io;
				io.m_nPort  = GetValientInt(&pDispParams->rgvarg[3]); //Port �ԍ�
				io.m_nNo	= GetValientInt(&pDispParams->rgvarg[2]); //���͔ԍ�
				io.m_nState = GetValientInt(&pDispParams->rgvarg[1]); //ON or OFF
				io.m_strFileName = pDispParams->rgvarg[0].bstrVal; //�X�N���v�g�t�@�C��				
				m_aryIOScript.Add(io);				
			}
			break;		
		case FUNC_STARTTIMER: //USBIO�^�C�}�[�ĊJ
			StartTimer();
			break;
		case FUNC_KILLTIMER: //USBIO�^�C�}�[
			KillTimer(); //�^�C�}�[�I��
			break;

	}
	return S_OK;
}

//�^�C�}�[�I��
void UsbIO::KillTimer()
{
	//GetFrame()->KillTimer(FUNC_SETTIMER); //�^�C�}�[�I��
}
//�^�C�}�[�ĊJ
void UsbIO::StartTimer()
{
	//GetFrame()->SetTimer(FUNC_SETTIMER,10,TimerProc); //�^�C�}�[�J�n
}

//USBIO ������
BOOL UsbIO::Init(BYTE byData1,BYTE byData2)
{
	m_hHid = OpenUsbIo(); //USB IO Open	
	if( m_hHid != NULL ){
		OutputUsbIo(m_hHid, 0, byData1); //Port0������
		OutputUsbIo(m_hHid, 1, byData2); //Port1������
		return TRUE;
	}
	return FALSE;
}

//�w��ԍ��ǂݍ���
int UsbIO::Read(int nPort,int nNo) 
{
	HANDLE hHid = GetHandle();
	if(hHid != NULL){
		BYTE byInput = InputUsbIo(hHid, nPort);
		if( (byInput & (1 << nNo)) == USBIO_ON ){ //�t���O�`�F�b�N
			return USBIO_ON; //ON�ł�
		}
	}		
	return USBIO_OFF; //OFF�ł�
}

//�w��o�͔ԍ��ɏ�������
void UsbIO::Write(int nPort,int nNo,int nState) 
{
	HANDLE hHid = GetHandle();
	if(hHid != NULL){
		BYTE byInput = InputUsbIo(hHid, nPort);
		if( nState == USBIO_ON ){
			byInput &= ~(1<<nNo); //�w��o�͔ԍ��݂̂O�ɂ���
		}else{
			byInput |= (1<<nNo); //�w��o�͔ԍ��̂�1�ɂ���
		}
		OutputUsbIo(m_hHid, nPort, byInput);
	}
}
//USBIO��ԕω��҂�
int UsbIO::Wait(int nPort,int nNo,int nState) 
{
	HANDLE hHid = GetHandle();
	if(hHid != NULL){
		KIBOFrame *pFrame = GetFrame();
		MSG msg;
		while( IsWindowOK() && Read(nPort,nNo) != nState ){ //nState�ɂȂ�܂�Wait
			if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
				if(msg.message == WM_CLOSE){
					return FALSE; //�����I��
				}
				if( msg.message == WM_KEYUP ){
					CString strFilePath;
					if( pFrame->m_mapKeyFilePath.Lookup((int)msg.wParam,strFilePath) ){ //�o�^���ꂽ�L�[���͂Ȃ��
						return -1;
					}
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		return TRUE;//����I��
	}
	return FALSE;//�ُ�I��
}

//---------------------------------------------------------
//�֐�		�FUSB-IO�I�[�v��
//�T�v		�F�ڑ�����USB-IO��S�ăI�[�v��
//�߂�l	�FUSB-IO�̃t�@�C���n���h��
//---------------------------------------------------------
static HANDLE OpenUsbIo()
{
	// Get hid GUID
	GUID hidGuid;
	HidD_GetHidGuid(&hidGuid); //HID �f�o�C�X��GUID �R�[�h���擾����B

	// Open PnP Handle
	// �f�o�C�X���񋓂��ꂽ���X�g�ւ́A�n���h�����擾����B
	HDEVINFO hdevinfo = SetupDiGetClassDevs( &hidGuid,NULL,NULL,(DIGCF_PRESENT|DIGCF_INTERFACEDEVICE) );
	if( hdevinfo == INVALID_HANDLE_VALUE ){
		_tprintf( _T("Error : SetupDiGetClassDevs() [%d]\n"), GetLastError() );
		return FALSE;
	}
	
	for(DWORD dwIndex=0; dwIndex<MAX_ENUM_INDEX; dwIndex++ ){ //�S�f�o�C�X����
		// Enum hid devices
		SP_INTERFACE_DEVICE_DATA device_data;
		ZeroMemory( &device_data, sizeof(device_data) );
		device_data.cbSize = sizeof(device_data);
		//���X�g�̐擪���珇��HID�f�o�C�X�̏����擾����B
		if( !SetupDiEnumDeviceInterfaces( hdevinfo, NULL, &hidGuid, dwIndex, &device_data ) ){
			break ; //����ȏ�f�o�C�X�Ȃ�
		}

		// Request SP_DEVICE_INTERFACE_DETAIL_DATA size		
		DWORD dwNeeded = 0;
		//�f�o�C�X�̏ڍ׏��T�C�Y�����߂�
		SetupDiGetDeviceInterfaceDetail( hdevinfo, &device_data, NULL, 0, &dwNeeded, NULL );
		DWORD dwSize = dwNeeded;
		if( dwSize > 1024 ){ //�f�o�C�X�T�C�Y�G���[
			continue ;
		}
		BYTE byData[1024]; //�ꎞ�I�ȃf�[�^�i�[�̈�
		PSP_DEVICE_INTERFACE_DETAIL_DATA pdevice_detail = (PSP_DEVICE_INTERFACE_DETAIL_DATA)byData;
		pdevice_detail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

		//�f�o�C�X�̏ڍ׏�񂩂�Windows �����̃f�o�C�X�����擾����B
		// Get SP_DEVICE_INTERFACE_DETAIL_DATA
		if( SetupDiGetDeviceInterfaceDetail( hdevinfo, &device_data, pdevice_detail, dwSize, &dwNeeded, NULL ) ){
			// for w2k patch
			SECURITY_ATTRIBUTES sa;
			sa.nLength = sizeof(SECURITY_ATTRIBUTES);
			sa.lpSecurityDescriptor = NULL;
			sa.bInheritHandle = FALSE;

			//HID�f�o�C�X���I�[�v�����A�t�@�C���n���h�����擾����B
			HANDLE hHid = CreateFile(pdevice_detail->DevicePath
									,GENERIC_READ|GENERIC_WRITE
									,FILE_SHARE_READ|FILE_SHARE_WRITE
									,&sa
									, OPEN_EXISTING, 0, NULL );

			if( hHid != INVALID_HANDLE_VALUE ){
				HIDD_ATTRIBUTES hidattr;
				BOOL bRet = HidD_GetAttributes( hHid, &hidattr ); //HID�f�o�C�X�����̎擾

				if( hidattr.VendorID == VENDOR_ID && hidattr.ProductID == PRODUCT_ID){
					return hHid; //�n���h���擾
				}else{
					CloseHandle(hHid); //�Ώ̊O�̃n���h���N���[�Y
				}
			}
		}
	}
	return NULL;
}

static PHIDP_PREPARSED_DATA GetHidData(HANDLE hHid, HIDP_CAPS &hidpCaps)
{
	PHIDP_PREPARSED_DATA phidpData;
	if( HidD_GetPreparsedData(hHid, &phidpData) ){ //HID �f�o�C�X�̔\�͂��擾
		NTSTATUS ntstat = HidP_GetCaps(phidpData, &hidpCaps);
		if( ntstat == HIDP_STATUS_SUCCESS ){
			_tprintf(_T("Info : Success HidP_GetCaps()\n"));
			_tprintf(_T("-----\n"));
			_tprintf(_T("  Usage     : [%d]\n"), hidpCaps.Usage );
			_tprintf(_T("  UsagePage : [%d]\n"), hidpCaps.UsagePage);
			_tprintf(_T("  InputReportByteLength    : [%d]\n"), hidpCaps.InputReportByteLength);
			_tprintf(_T("  OutputReportByteLength   : [%d]\n"), hidpCaps.OutputReportByteLength);
			_tprintf(_T("  InputReportByteLength    : [%d]\n"), hidpCaps.InputReportByteLength);
			_tprintf(_T("  OutputReportByteLength   : [%d]\n"), hidpCaps.OutputReportByteLength);
			_tprintf(_T("  FeatureReportByteLength  : [%d]\n"), hidpCaps.FeatureReportByteLength);
			_tprintf(_T("  NumberLinkCollectionNodes: [%d]\n"), hidpCaps.NumberLinkCollectionNodes);
			_tprintf(_T("  NumberInputButtonCaps    : [%d]\n"), hidpCaps.NumberInputButtonCaps);
			_tprintf(_T("  NumberInputValueCaps     : [%d]\n"), hidpCaps.NumberInputValueCaps);
			_tprintf(_T("  NumberInputDataIndices   : [%d]\n"), hidpCaps.NumberInputDataIndices);
			_tprintf(_T("  NumberOutputButtonCaps   : [%d]\n"), hidpCaps.NumberOutputButtonCaps);
			_tprintf(_T("  NumberOutputValueCaps    : [%d]\n"), hidpCaps.NumberOutputValueCaps);
			_tprintf(_T("  NumberOutputDataIndices  : [%d]\n"), hidpCaps.NumberOutputDataIndices);
			_tprintf(_T("  NumberFeatureButtonCaps  : [%d]\n"), hidpCaps.NumberFeatureButtonCaps);
			_tprintf(_T("  NumberFeatureValueCaps   : [%d]\n"), hidpCaps.NumberFeatureValueCaps);
			_tprintf(_T("  NumberFeatureDataIndices : [%d]\n"), hidpCaps.NumberFeatureDataIndices);
			_tprintf(_T("-----\n"));
			return phidpData;
		}
	}
	return NULL;
}


//---------------------------------------------------------
//�֐�		�FUSB-IO�֑��M
//����		�Fint �n���h��
//			�Fint �R�}���h
//			�Fint �f�[�^
//			�Fint �J�E���^
//�T�v		�FUSB-IO�փR�}���h�𑗐M
//�߂�l	�FFALSE=NG	0�ȊO OK
//---------------------------------------------------------
static BOOL SendUsbIoCmd(HANDLE hHid, int iCmd, int iData, int iCounter)
{
	BYTE byData[9]; //9Byte ���M
	byData[0] = 0x00;
	byData[1] = iCmd & 0xff;
	byData[2] = iData & 0xff;
	byData[3] = 0xF3;
	byData[4] = 0xF4;
	byData[5] = 0xF5;
	byData[6] = 0xF6;
	byData[7] = iCounter & 0xff;
	byData[8] = 0xF8;

	DWORD dwWritten = 9;
	if( !WriteFile( hHid, byData, dwWritten, &dwWritten, NULL) ){
		char szMsgBuf[1024];
		ZeroMemory(szMsgBuf,sizeof(szMsgBuf));
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_IGNORE_INSERTS
						,NULL
						,GetLastError()
						,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT) // ����̌���
						,(LPTSTR) szMsgBuf
						,0
						,NULL
						);
		_tprintf(_T("Error: %hs\n"),szMsgBuf);
	}
	return (dwWritten==sizeof(byData));
}

//---------------------------------------------------------
//�֐�		�FUSB-IO �o��
//����		�FHANDLE hHid �I�[�v������UsbIo Handle
//			�Fint iPort		�o�̓|�[�g 0 or 1
//			�Fint iData		�o�̓f�[�^
//�T�v		�FUSB-IO �̎w��|�[�g���o��
//�߂�l	�FTRUE �o�͐��� ���̑��G���[
//---------------------------------------------------------
static BOOL OutputUsbIo(HANDLE hHid,int iPort, int iData)
{
	if( iPort == 0 || iPort == 1 ){
		if( (iData > 0xff) || (iData < 0) ){ //Data Size Check
			return FALSE;
		}
		return SendUsbIoCmd( hHid,iPort+1,iData,0 ); //Output�̏ꍇ��Port�ԍ�+1 == �R�}���h�ԍ�
	}
	return FALSE;
}


//---------------------------------------------------------
//�֐�		�FUSB-IO�f�[�^�ǂݍ���
//����		�Fint �n���h��
//			�Fint �f�[�^���ʃJ�E���^
//�T�v		�FUSB-IO�փR�}���h�𑗐M
//�߂�l	�F0 �ȏ� ���͒l		0 ���� �G���[
//---------------------------------------------------------
static BYTE ReadUsbIoData(HANDLE hHid, int iCounter)
{
	BYTE byRData[9]; //9Byte Data
	DWORD dwRead = 0;
	ReadFile(hHid, byRData, 9, &dwRead , 0); //Data�ǂݍ���

	for(int iWaitRetry=0;iWaitRetry<15;iWaitRetry++){
		for(int iImmediateRetry=0;iImmediateRetry<10;iImmediateRetry++){
			ReadFile(hHid, byRData, 9, &dwRead , 0); //Data�ǂݍ���
			int iRetCounter = byRData[7];
			int iRetCmd		= byRData[1];
			if( iRetCounter == iCounter && (iRetCmd == 3 || iRetCmd == 4) ){
				ReadFile(hHid, byRData, 9, &dwRead , 0); //Data�ǂݍ���
				return byRData[2];
			}
		}
		Sleep(1);
	}
	
	return 0;
}


//---------------------------------------------------------
//�֐�		�FUSB-IO ���[�h
//����		�Fint iUsbioNo	�I�[�v������UsbIo 0�`
//			�Fint iPort		���̓|�[�g 0 or 1
//�T�v		�F�w�肵��USB-IO�ԍ��ڑ�����USB-IO��S�ăI�[�v��
//�߂�l	�F0 �ȏ� ���͒l		0 ���� �G���[
//---------------------------------------------------------
static BYTE InputUsbIo(HANDLE hHid, int iPort)
{
	static int s_iUsbIoSendCnt = 0;

	int iCmd = 0;
	switch(iPort){ //Input �R�}���h�ɕύX
		case 0:
			iCmd = 3;
			break;
		case 1:
			iCmd = 4;
			break;
		default:
			return 0;
	}

	s_iUsbIoSendCnt++;
	if (s_iUsbIoSendCnt > 0xff) {
		s_iUsbIoSendCnt = 1;
	}

	if( !SendUsbIoCmd( hHid, iCmd, 0, s_iUsbIoSendCnt) ){
		return 0;
	}
	return ReadUsbIoData(hHid, s_iUsbIoSendCnt);
}

#endif