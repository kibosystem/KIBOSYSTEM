#ifndef __ICubeSDK_H__
#define __ICubeSDK_H__

#include <windows.h>

//---------------------------------------------------------------------------
// Global Defines

#define		IC_SUCCESS			0
#define		IC_ERROR			1

#define		IC_IF_NOT_OPEN		-1
#define		IC_WRONG_PARAM		-2
#define		IC_OUT_OF_MEMORY	-3
#define		IC_ALREADY_DONE		-4
#define		IC_WRONG_CLOCK_VAL	-5
#define		IC_COM_LIB_INIT    	-6
#define		IC_NOT_IF_STARTED  	-7
#define		IC_WRONG_ROI_ID  	-8
#define		IC_IF_NOT_ENABLED  	-9		//
#define		IC_COLOR_CAM_ONLY	-10		//

// Video Mode definitions
// (used with: ICubeSDK_SetMode,ICubeSDK_GetMode)

#define		MODE_320x240		0
#define		MODE_640x480		1
#define		MODE_752x480		2
#define		MODE_800x600		3
#define		MODE_1024x768		4
#define		MODE_1280x1024		5
#define		MODE_1600x1200		6
#define		MODE_2048x1536		7
#define		MODE_2592x1944		8
#define		MODE_3840x2748		9

// BinSkip definitions
// (used with: ICubeSDK_SetBinSkip,ICubeSDK_GetBinSkip,ICubeSDK_GetBinSkipList)

#define     MODE_SKIP		     0
#define     MODE_BIN		     1
#define     BIN_SKIP_OFF    	 0
#define     BIN_SKIP_2ND_PIXEL	 1
#define     BIN_SKIP_4TH_PIXEL	 2


// Parameter definitions 
// (used with: ICubeSDK_SetCamParameter,ICubeSDK_GetCamParameter,
// ICubeSDK_GetCamParameterRange,ICubeSDK_GetParamAuto,ICubeSDK_SetParamAuto,
// ICubeSDK_SetParamAutoDef,ICubeSDK_SetParamOnePush)

// Measure field definitions
// (used with REG_MEASURE_FIELD_AE)

#define MEASUREFIELD_100	     2	// 100% (full frame,default)
#define MEASUREFIELD_60			 1	// 60%
#define MEASUREFIELD_30			 0	// 30%

// Shutter definitions
// (used with REG_SHUTTER)

#define SHUTTER_ROLLING	        0	// all models
#define SHUTTER_GLOBAL_RESET	1	// (4133CU/BU,4133CU only)
#define SHUTTER_GLOBAL			2	// (4133CU/BU (default),4133CU (default) only)

// Parameter ON||OFF definitions

#define OFF                     0
#define ON                      1

// Trigger invert definitions
// hardware trigger will be detected on the defined edge
// (used with REG_TRIGGER_INVERT)

#define FALLING_EDGE			0	// (default)
#define RISING_EDGE				1	

// Software trigger mode definitions
// (used with REG_SW_TRIG_MODE)

#define DELAYED_TRIGGER_RETURN   0  // camera checks for frame arrival (default),
									// ICubeSDK_SetTrigger(..,TRIG_SW_DO) returns this status
#define IMMEDIATE_TRIGGER_RETURN 1	// no camera check

// Bayer Conversion definitions
// (used with REG_BAYER_CONVERSION)

#define DE_BAYER_3x3			 0          
#define DE_BAYER_5x5			 1 // (default)           

// Gamma definitions
// (used with REG_GAMMA_MODE)

#define GAMMA_MODE_DEFAULT   	     2	// (default)   
#define GAMMA_MODE_sRGB  			 1
#define GAMMA_MODE_ITU709			 0	// (HDTV)


// Camera parameters definitions

#define REG_BRIGHTNESS			1	// write: only when REG_COLOR_PROCCESSING enabled, read: always
#define REG_CONTRAST			2	// write: only when REG_COLOR_PROCCESSING enabled, read: always
#define REG_GAMMA				3   // write: only when REG_GAMMA_ENABLE enabled, read: always
#define REG_FLIPPED_V			4	// (ON||OFF); OFF (default)
#define REG_FLIPPED_H			5	// (ON||OFF); OFF (default)
#define REG_WHITE_BALANCE		6	// (one push)
#define REG_EXPOSURE_TIME		7   // all models
#define REG_EXPOSURE_TARGET		8	// sets the target-value for the auto exposure algorithm 
#define REG_RED					9	// only for color models; RGB Gain value
#define REG_GREEN				10	// only for color models
#define REG_BLUE				11	// only for color models
#define REG_BLACKLEVEL			12  // sensor blacklevel
#define REG_GAIN				13	// sensor gain
#define REG_COLOR				14	// (ON||OFF) read: color || bw model
#define REG_PLL					15	// all models
#define REG_STROBE_LENGHT		16	// length of strobe pulse output (msec)
#define REG_STROBE_DELAY		17  // delay before strobe pulse is executed (msec)
#define REG_TRIGGER_DELAY		18  // delay before hardware trigger is executed (msec)
#define REG_SATURATION			19	// only for color models, when REG_COLOR_PROCCESSING enabled 
#define REG_COLOR_ENHANCE		20	// obsolet
#define REG_TRIGGER_INVERT		21	// (FALLING_EDGE (default) || RISING_EDGE)
#define REG_RECONNECTIONS		22	// get the value of usb reconnections
#define REG_MEASURE_FIELD_AE	23  // measure field for auto exposure
#define REG_AVI_STATE			24	// get the avi stream save status (ON||OFF)
#define REG_FOCUS				25  // not implemented yet
#define REG_SHUTTER				26  // (4133CU/BU,4133CU)
#define REG_ROI_ID				27	// (4133CU/BU,4133CU,fpga board cameras)
#define REG_ROI_CYCLE			28  // (4133CU/BU,4133CU) number of repetitions of the particular roi

#define REG_DEFECT_COR			43  // (4133CU/BU,4133CU) DefectPixelCorrection (ON||OFF); OFF(default)
#define REG_BAD_FRAME           81  // read bad frame count (also possible with ICubeSDK_GetBrokenFrames)
#define REG_GOOD_FRAME          82  // read good frame count (also possible with ICubeSDK_GetGoodFrames)
#define REG_SW_TRIG_MODE		94  // (DELAYED_TRIGGER_RETURN (default) || IMMEDIATE_TRIGGER_RETURN)
#define REG_ROI_FPGA_ONE_FRAME	96	// (fpga board only) pseudo frame format (ON||OFF)	
#define REG_CALLBACK_BR_FRAMES	97	// Broken frames also triggering the callback function (ON||OFF); OFF(default)
#define REG_FGPA_VBLANKING		98	// (fpga board only) output Frame blanking
#define REG_FGPA_HBLANKING		99  // (fpga board only) output Frame blanking
#define REG_FGPA_CLK_DIVIDER	100 // (fpga board only) output clock divider (ON||OFF)
#define REG_FGPA_ON_BOARD		101 // check if camera has fpga board (ON||OFF)

#define REG_SET_GPIO			102 // bit masked: XXXX X J3-7 J3-6 J3-5
#define REG_GET_GPIO			103 // bit masked: XXXX X J3-7 J3-6 J3-5
#define REG_SET_GPIO_MODE		104	// (0==Input || 1==Output),bit masked: XXXX X J3-7 J3-6 J3-5

#define REG_MASK_ROI_ID         105 // roi_id masked in buffersize of callback 

#define REG_RED_OFFSET			106	// only for color models; RGB offset value
#define REG_GREEN_OFFSET		107 // only for color models
#define REG_BLUE_OFFSET			108 // only for color models

#define REG_HUE					109	// only for color models; write: only when REG_COLOR_PROCCESSING enabled, read: always
#define REG_COLOR_CORRECTION	110 // (ON||OFF) ColorCorrection matrix; OFF (default)
#define REG_GAMMA_ENABLE		111 // (ON||OFF) en/disable gamma module to save CPU load; ON (default)
#define REG_GAMMA_MODE			112 // only for color models ; write: only when REG_GAMMA_ENABLE enabled, read: always
#define REG_INVERT_PIXEL		113 // (ON||OFF)  bw cams only
#define REG_TNR					115 // (ON||OFF)  only for color models, temporal noise reduction
#define REG_BAYER_CONVERSION	116 // only for color models; choose different debayering
#define REG_COLOR_PROCESSING	117	// (ON||OFF) en/disable color proc module module to save CPU load;  ON (default)	


// Parameter definitions
// (used with: ICubeSDK_GetCamParameterRange)

typedef struct {
	BOOL	bEnabled;
	BOOL	bAuto;
	BOOL	bOnePush;
	UINT	nDef;
	UINT	nMin;
	ULONG	nMax;	
} 
PARAM_PROPERTY, *P_PARAM_PROPERTY;

#define DEFAULT_GAMMA			64


// Parameter definitions (float value)
// (used with: ICubeSDK_GetExposureRange)
// values in [msec]

typedef struct {
	BOOL	bEnabled;
	BOOL	bAuto;
	BOOL	bOnePush;
	FLOAT	nDef;
	FLOAT	nMin;
	FLOAT	nMax;	
} 
PARAM_PROPERTY_f, *P_PARAM_PROPERTY_f;

// Roi definitions
// (used with: REG_ROI_ID)

// ROI ID: (4133CU/BU)
// SIMR: Single Integration Multiple Roi  (4133CU/BU only)
// 1, 2 or 4 areas of interest to be acquired within the same integrated image
// MIMR: Multible Integration Multiple Roi (4133CU/BU only)
// 
#define ROI_ID_1_2	        0	// SIMR (4133CU/BU only)
#define ROI_ID_1_1	        1	// default mode for all models (1st roi)
#define ROI_ID_2	        2   // MIMR additonal (4133CU/BU only)
#define ROI_ID_3	        3
#define ROI_ID_4	        4

// ROI ID: (all cameras with fpga board)
//
#define ROI_ID_0	        0	// fpga input size
#define ROI_ID_1	        1	// fpga output size roi 1
#define ROI_ID_2	        2   
#define ROI_ID_3	        3
#define ROI_ID_4	        4
#define ROI_ID_5	        5

// ROI ID: (all cameras without fpga board,except 4133CU/BU)
// only one roi available (roi1)

// (used with: ICubeSDK_SetResolution, ICubeSDK_GetResolution)

typedef struct {
	BOOL	bEnabled;
	INT		nXRes;
	INT		nYRes;
	INT		nXPos;
	INT		nYPos;
} 
ROI_PROPERTY, *P_ROI_PROPERTY;

// (used with: ICubeSDK_GetResolutionRange)

typedef struct {
	INT		nXMin;
	INT		nXMax;
	INT		nYMin;
	INT		nYMax;
}
ROI_RANGE_PROPERTY, *P_ROI_RANGE_PROPERTY;


// Trigger definitions
// (used with: ICubeSDK_SetTrigger, ICubeSDK_GetTrigger)

#define TRIG_SW_START			0		// start software trigger
#define TRIG_SW_DO		        1		// do a software trigger pulse
#define TRIG_HW_START           2		// start hardware trigger
#define TRIG_STOP               3		// stop software || hardware trigger
#define TRIG_SW_START_2			4    	// GlobalResetRelease shutter mode (1500CU/BU,1300CU)
#define TRIG_HW_START_2         5    	// GlobalResetRelease shutter mode (1500CU/BU,1300CU)


// Versions definitions
// (used with: ICubeSDK_GetVersion)

#define	DLL_VERSION				0		// (ICubeSDK.dll)
#define API_VERSION				1		// (NET_USB_CMO.dll)

// Callback definitions
// (used with: ICubeSDK_SetCallback)

#define CALLBACK_RAW            0		// 8bit/pixel data is received
										// (if the camera is a color model data == raw bayer)
#define CALLBACK_RGB            1		// 24bit/pixel data is received


// DisplayMode definitions
// (used with: ICubeSDK_SetDisplayMode)

#define DISPLAY_NORMAL			0		// frame is previewed in original size (default)
#define DISPLAY_FIT_TO_WINDOW	1		// frame is previewed in resized mode (window size) 
#define DISPLAY_RECT			2		// frame is previewed in resized mode (rect size)

typedef struct {
	INT    left;
    INT    top;
    INT    right;
    INT    bottom;
} 
DISP_PROPERTY, *P_DISP_PROPERTY;


// AVI stream definitions
// (used with: ICubeSDK_SaveAvi)

#define AVI_DIB				    0	// currently, only this avi mode is supported


// Name length definitions
// (used with: ICubeSDK_GetName,ICubeSDK_GetSerialNum,ICubeSDK_GetVersion,ICubeSDK_GetFWVersion)

#define NETCAM_NAME_LENGTH		13	// length of the name string
#define NETCAM_SERIAL_LENGTH	10	// length of the serial number string
#define NETCAM_VERSION_LENGTH   9	// length of the version string


// IsOpen definitions
// (used with: ICubeSDK_IsOpen)

#define IS_CLOSED               0
#define IS_OPEN                 1

// IsStarted definitions
// (used with: ICubeSDK_IsStarted)

#define IS_STOPPED              0
#define IS_STARTED              1


//---------------------------------------------------------------------------
//#define LIBFILE		// load DLL with libfile

//---------------------------------------------------------------------------
#ifdef LIBFILE       

//#define BORLAND_C		// if used with borland c++ builder -> uncomment

#define ICubeSDK

#ifdef BORLAND_C
extern "C"
{
#endif

//////////////////////////////////////////////////////////////////////////////'
//
//          Camera control functions
//
//////////////////////////////////////////////////////////////////////////////

ICubeSDK int	WINAPI ICubeSDK_Init (void);
ICubeSDK int	WINAPI ICubeSDK_Open(IN INT nCamIndex);
ICubeSDK int	WINAPI ICubeSDK_Close(IN INT nCamIndex);
ICubeSDK int	WINAPI ICubeSDK_IsOpen(IN INT nCamIndex);

ICubeSDK int	WINAPI ICubeSDK_SetCallback(IN INT nCamIndex,IN INT nMode,IN LONG (CALLBACK *pCallbackFunc)(BYTE *pBuf, LONG lBufferSize, PVOID pContext),
                                                   IN VOID *pCBContext);

ICubeSDK int	WINAPI ICubeSDK_Start(IN INT nCamIndex,HWND ImgHandle,IN BOOL Preview,IN BOOL Callback);
ICubeSDK int	WINAPI ICubeSDK_IsStarted(IN INT nCamIndex);
ICubeSDK int	WINAPI ICubeSDK_Stop(IN INT nCamIndex);
ICubeSDK int	WINAPI ICubeSDK_GetSize(IN INT nCamIndex,OUT PINT pnXRes,OUT PINT pnYRes);
ICubeSDK int	WINAPI ICubeSDK_GetName(IN INT nCamIndex,OUT PCHAR Name);

ICubeSDK int	WINAPI ICubeSDK_GetBrokenFrames(IN INT nCamIndex,OUT PINT pnFrames);
ICubeSDK int	WINAPI ICubeSDK_GetGoodFrames(IN INT nCamIndex,OUT PINT pnFrames);

ICubeSDK int	WINAPI ICubeSDK_SetDisplayMode(IN INT nCamIndex,IN INT nMode,DISP_PROPERTY property);

//////////////////////////////////////////////////////////////////////////////'
//
//          Version functions
//
//////////////////////////////////////////////////////////////////////////////

ICubeSDK void	WINAPI ICubeSDK_GetVersion(IN INT nType,OUT PCHAR pVersion);
ICubeSDK int	WINAPI ICubeSDK_GetFWVersion(IN INT nCamIndex,OUT PCHAR pVersion);
ICubeSDK int	WINAPI ICubeSDK_GetSerialNum(IN INT nCamIndex,OUT PCHAR pVersion);
ICubeSDK int	WINAPI ICubeSDK_GetFGPAVersion(IN INT nCamIndex,OUT PCHAR pVersion);

//////////////////////////////////////////////////////////////////////////////'
//
//          ROI functions
//
//////////////////////////////////////////////////////////////////////////////

ICubeSDK int	WINAPI ICubeSDK_SetResolution(IN INT nCamIndex,ROI_PROPERTY *property);
ICubeSDK int	WINAPI ICubeSDK_GetResolution(IN INT nCamIndex,ROI_PROPERTY *property);
ICubeSDK int	WINAPI ICubeSDK_GetResolutionRange(IN INT nCamIndex,ROI_RANGE_PROPERTY *property);

ICubeSDK int	WINAPI ICubeSDK_SetResolutionParam(IN INT nCamIndex,HWND ImgHandle,IN BOOL Preview,IN BOOL Callback);

//////////////////////////////////////////////////////////////////////////////'
//
//          Mode functions
//
//////////////////////////////////////////////////////////////////////////////

ICubeSDK int	WINAPI ICubeSDK_SetMode(IN INT nCamIndex,IN INT nMode);
ICubeSDK int	WINAPI ICubeSDK_GetMode(IN INT nCamIndex,OUT PINT pMode);
ICubeSDK int	WINAPI ICubeSDK_GetModeList(IN INT nCamIndex,OUT PINT pLength,OUT PINT pList);

//////////////////////////////////////////////////////////////////////////////'
//
//          Bin Skip functions
//
//////////////////////////////////////////////////////////////////////////////

ICubeSDK int	WINAPI ICubeSDK_SetBinSkip(IN INT nCamIndex,IN INT nParameter,IN INT nMode);
ICubeSDK int	WINAPI ICubeSDK_GetBinSkip(IN INT nCamIndex,OUT PINT nParameter,IN INT nMode);
ICubeSDK int	WINAPI ICubeSDK_GetBinSkipList(IN INT nCamIndex,IN INT nMode,OUT PINT pLenght,OUT PINT pList);

//////////////////////////////////////////////////////////////////////////////'
//
//          Save functions
//
//////////////////////////////////////////////////////////////////////////////

ICubeSDK int	WINAPI ICubeSDK_SaveToFile(IN INT nCamIndex,IN  CHAR* Name);
ICubeSDK int	WINAPI ICubeSDK_SaveAvi(IN INT nCamIndex,IN  CHAR* Name,IN INT nMode,IN INT nTimeInSecs);

//////////////////////////////////////////////////////////////////////////////'
//
//          Trigger functions
//
//////////////////////////////////////////////////////////////////////////////

ICubeSDK int	WINAPI ICubeSDK_SetTrigger(IN INT nCamIndex,IN INT nMode);
ICubeSDK int	WINAPI ICubeSDK_GetTrigger(IN INT nCamIndex,OUT PINT nMode);

//////////////////////////////////////////////////////////////////////////////'
//
//          Parameter functions 
//
//////////////////////////////////////////////////////////////////////////////

ICubeSDK int	WINAPI ICubeSDK_SetCamParameter(IN INT nCamIndex,IN INT Type,IN ULONG Value);
ICubeSDK int	WINAPI ICubeSDK_GetCamParameter(IN INT nCamIndex,IN INT Type,OUT PULONG Value);
ICubeSDK int	WINAPI ICubeSDK_GetCamParameterRange(IN INT nCamIndex,IN INT Type,PARAM_PROPERTY *property);

//////////////////////////////////////////////////////////////////////////////'
//
//          Exposure functions 
//          This functions can be used alternativly to parameter functions,
//          for manipulating exposure only. Parameter Value is in milli-seconds.
//          Be aware that every change of clock
//          and width will lead to different exposure values
//
//////////////////////////////////////////////////////////////////////////////

ICubeSDK int	WINAPI ICubeSDK_SetExposure(IN INT nCamIndex,IN FLOAT Value);
ICubeSDK int	WINAPI ICubeSDK_GetExposure(IN INT nCamIndex,OUT PFLOAT Value);
ICubeSDK int	WINAPI ICubeSDK_GetExposureRange(IN INT nCamIndex,PARAM_PROPERTY_f *property);

//////////////////////////////////////////////////////////////////////////////'
//
//       for Parameter functions and Exposure functions (auto||manual,one-push)
//
//////////////////////////////////////////////////////////////////////////////

ICubeSDK int	WINAPI ICubeSDK_GetParamAuto(IN INT nCamIndex,IN INT Type,OUT PINT bAuto);
ICubeSDK int	WINAPI ICubeSDK_SetParamAuto(IN INT nCamIndex,IN INT Type,IN BOOL bAuto);
ICubeSDK int	WINAPI ICubeSDK_SetParamAutoDef(IN INT nCamIndex,IN INT Type);
ICubeSDK int	WINAPI ICubeSDK_SetParamOnePush(IN INT nCamIndex,IN INT Type);

#ifdef BORLAND_C
}
#endif
//------------------------------------------------------------------------------------------------------------------------------------------------
#else        // load DLL without libfile

typedef int (WINAPI *LPICubeSDK_Init)(void);
typedef LPICubeSDK_Init PICubeSDK_Init;

//------------------------------------------------------------------------------------

typedef int (WINAPI *LPICubeSDK_Open)(IN INT nCamIndex);
typedef LPICubeSDK_Open PICubeSDK_Open;

typedef int (WINAPI *LPICubeSDK_Close)(IN INT nCamIndex);
typedef LPICubeSDK_Close PICubeSDK_Close;

typedef int (WINAPI *LPICubeSDK_IsOpen)(IN INT nCamIndex);
typedef LPICubeSDK_IsOpen PICubeSDK_IsOpen;

//------------------------------------------------------------------------------------

typedef void (WINAPI *LPICubeSDK_GetVersion)(IN INT nType,OUT PCHAR pVersion);
typedef LPICubeSDK_GetVersion PICubeSDK_GetVersion;

typedef int (WINAPI *LPICubeSDK_GetFWVersion)(IN INT nCamIndex,OUT PCHAR pVersion);
typedef LPICubeSDK_GetFWVersion PICubeSDK_GetFWVersion;

typedef int (WINAPI *LPICubeSDK_GetSerialNum)(IN INT nCamIndex,OUT PCHAR pVersion);
typedef LPICubeSDK_GetSerialNum PICubeSDK_GetSerialNum;

typedef int (WINAPI *LPICubeSDK_GetFPGAVersion)(IN INT nCamIndex,OUT PCHAR pVersion);
typedef LPICubeSDK_GetFPGAVersion PICubeSDK_GetFPGAVersion;

//------------------------------------------------------------------------------------
typedef int (WINAPI *LPICubeSDK_SetCallback)(IN INT nCamIndex,IN INT nMode,IN LONG (CALLBACK *pCallbackFunc)(BYTE *pBuf, LONG lBufferSize, PVOID pContext),
                                                  IN VOID *pCBContext);
typedef LPICubeSDK_SetCallback PICubeSDK_SetCallback;

typedef int (WINAPI *LPICubeSDK_Start)(IN INT nCamIndex,HWND ImgHandle,IN BOOL Preview,IN BOOL Callback);
typedef LPICubeSDK_Start PICubeSDK_Start;

typedef int (WINAPI *LPICubeSDK_IsStarted)(IN INT nCamIndex);
typedef LPICubeSDK_IsStarted PICubeSDK_IsStarted;

typedef int (WINAPI *LPICubeSDK_Stop)(IN INT nCamIndex);
typedef LPICubeSDK_Stop PICubeSDK_Stop;

typedef int (WINAPI *LPICubeSDK_SetDisplayMode) (IN INT nCamIndex,IN INT nMode,DISP_PROPERTY property);
typedef LPICubeSDK_SetDisplayMode PICubeSDK_SetDisplayMode;

//------------------------------------------------------------------------------------
typedef int (WINAPI *LPICubeSDK_SetResolution) (IN INT nCamIndex,ROI_PROPERTY *property);
typedef LPICubeSDK_SetResolution PICubeSDK_SetResolution;

typedef int (WINAPI *LPICubeSDK_GetResolution) (IN INT nCamIndex,ROI_PROPERTY *property);
typedef LPICubeSDK_GetResolution PICubeSDK_GetResolution;

typedef int (WINAPI *LPICubeSDK_GetResolutionRange) (IN INT nCamIndex,ROI_RANGE_PROPERTY *property);
typedef LPICubeSDK_GetResolutionRange PICubeSDK_GetResolutionRange;

typedef int (WINAPI *LPICubeSDK_SetResolutionParam) (IN INT nCamIndex,HWND ImgHandle,IN BOOL Preview,IN BOOL Callback);
typedef LPICubeSDK_SetResolutionParam PICubeSDK_SetResolutionParam;

typedef int (WINAPI *LPICubeSDK_GetSize) (IN INT nCamIndex,OUT PINT pnXRes,OUT PINT pnYRes);
typedef LPICubeSDK_GetSize PICubeSDK_GetSize;

//------------------------------------------------------------------------------------
typedef int (WINAPI *LPICubeSDK_SetMode) (IN INT nCamIndex,IN INT nMode);
typedef LPICubeSDK_SetMode PICubeSDK_SetMode;

typedef int (WINAPI *LPICubeSDK_GetMode) (IN INT nCamIndex,OUT PINT pMode);
typedef LPICubeSDK_GetMode PICubeSDK_GetMode;

typedef int (WINAPI *LPICubeSDK_GetModeList) (IN INT nCamIndex,OUT PINT pLength,OUT PINT pList);
typedef LPICubeSDK_GetModeList PICubeSDK_GetModeList;

//------------------------------------------------------------------------------------
typedef int (WINAPI *LPICubeSDK_SetBinSkip) (IN INT nCamIndex,IN INT nParameter,IN INT nMode);
typedef LPICubeSDK_SetBinSkip PICubeSDK_SetBinSkip;

typedef int (WINAPI *LPICubeSDK_GetBinSkip) (IN INT nCamIndex,OUT PINT nParameter,IN INT nMode);
typedef LPICubeSDK_GetBinSkip PICubeSDK_GetBinSkip;

typedef int (WINAPI *LPICubeSDK_GetBinSkipList) (IN INT nCamIndex,IN INT nMode,OUT PINT pLenght,OUT PINT pList);
typedef LPICubeSDK_GetBinSkipList PICubeSDK_GetBinSkipList;

//------------------------------------------------------------------------------------
typedef int (WINAPI *LPICubeSDK_SaveToFile)(IN INT nCamIndex,IN CONST CHAR* Name);
typedef LPICubeSDK_SaveToFile PICubeSDK_SaveToFile;

typedef int (WINAPI *LPICubeSDK_SaveAvi)(IN INT nCamIndex,IN CONST CHAR* Name,IN INT nMode,IN INT nTimeInSecs);
typedef LPICubeSDK_SaveAvi PICubeSDK_SaveAvi;

//------------------------------------------------------------------------------------

typedef int (WINAPI *LPICubeSDK_SetTrigger)(IN INT nCamIndex,IN INT nMode);
typedef LPICubeSDK_SetTrigger PICubeSDK_SetTrigger;

typedef int (WINAPI *LPICubeSDK_GetTrigger)(IN INT nCamIndex,OUT PINT nMode);
typedef LPICubeSDK_GetTrigger PICubeSDK_GetTrigger;

//------------------------------------------------------------------------------------

typedef int (WINAPI *LPICubeSDK_SetCamParameter) (IN INT nCamIndex,IN INT Type,IN ULONG Value);
typedef LPICubeSDK_SetCamParameter PICubeSDK_SetCamParameter;

typedef int (WINAPI *LPICubeSDK_GetCamParameter) (IN INT nCamIndex,IN INT Type,OUT PULONG Value);
typedef LPICubeSDK_GetCamParameter PICubeSDK_GetCamParameter;

typedef int (WINAPI *LPICubeSDK_GetCamParameterRange) (IN INT nCamIndex,IN INT Type,PARAM_PROPERTY *property);
typedef LPICubeSDK_GetCamParameterRange PICubeSDK_GetCamParameterRange;

typedef int (WINAPI *LPICubeSDK_GetParamAuto) (IN INT nCamIndex,IN INT Type,OUT PINT bAuto);
typedef LPICubeSDK_GetParamAuto PICubeSDK_GetParamAuto;

typedef int (WINAPI *LPICubeSDK_SetParamAuto) (IN INT nCamIndex,IN INT Type,IN BOOL bAuto);
typedef LPICubeSDK_SetParamAuto PICubeSDK_SetParamAuto;

typedef int (WINAPI *LPICubeSDK_SetParamAutoDef) (IN INT nCamIndex,IN INT Type);
typedef LPICubeSDK_SetParamAutoDef PICubeSDK_SetParamAutoDef;

typedef int (WINAPI *LPICubeSDK_SetParamOnePush) (IN INT nCamIndex,IN INT Type);
typedef LPICubeSDK_SetParamOnePush PICubeSDK_SetParamOnePush;
//------------------------------------------------------------------------------------
typedef int (WINAPI *LPICubeSDK_GetName) (IN INT nCamIndex,OUT PCHAR Name);
typedef LPICubeSDK_GetName PICubeSDK_GetName;
//------------------------------------------------------------------------------------
typedef int (WINAPI *LPICubeSDK_GetBrokenFrames) (IN INT nCamIndex,OUT PINT pnFrames);
typedef LPICubeSDK_GetBrokenFrames PICubeSDK_GetBrokenFrames;

typedef int (WINAPI *LPICubeSDK_GetGoodFrames) (IN INT nCamIndex,OUT PINT pnFrames);
typedef LPICubeSDK_GetGoodFrames PICubeSDK_GetGoodFrames;
//------------------------------------------------------------------------------------
typedef int (WINAPI *LPICubeSDK_SetExposure) (IN INT nCamIndex,IN FLOAT Value);
typedef LPICubeSDK_SetExposure PICubeSDK_SetExposure;

typedef int (WINAPI *LPICubeSDK_GetExposure) (IN INT nCamIndex,OUT PFLOAT Value);
typedef LPICubeSDK_GetExposure PICubeSDK_GetExposure;

typedef int (WINAPI *LPICubeSDK_GetExposureRange) (IN INT nCamIndex,PARAM_PROPERTY_f *property);
typedef LPICubeSDK_GetExposureRange PICubeSDK_GetExposureRange;


extern BOOL LoadICubeApi(const char* szDllPathname);
extern VOID UnloadICubeApi();
extern PICubeSDK_Init					ICubeSDK_Init;
extern PICubeSDK_Open					ICubeSDK_Open;
extern PICubeSDK_Close					ICubeSDK_Close;
extern PICubeSDK_IsOpen					ICubeSDK_IsOpen;
extern PICubeSDK_GetVersion				ICubeSDK_GetVersion;
extern PICubeSDK_GetFWVersion			ICubeSDK_GetFWVersion;
extern PICubeSDK_GetFPGAVersion			ICubeSDK_GetFPGAVersion;
extern PICubeSDK_GetSerialNum           ICubeSDK_GetSerialNum;
extern PICubeSDK_Stop					ICubeSDK_Stop;
extern PICubeSDK_SetCallback			ICubeSDK_SetCallback;
extern PICubeSDK_Start					ICubeSDK_Start;
extern PICubeSDK_IsStarted				ICubeSDK_IsStarted;
extern PICubeSDK_SetResolution			ICubeSDK_SetResolution;
extern PICubeSDK_GetResolution			ICubeSDK_GetResolution;
extern PICubeSDK_GetResolutionRange		ICubeSDK_GetResolutionRange;
extern PICubeSDK_GetSize				ICubeSDK_GetSize;
extern PICubeSDK_SetMode				ICubeSDK_SetMode;
extern PICubeSDK_GetMode				ICubeSDK_GetMode;
extern PICubeSDK_GetModeList			ICubeSDK_GetModeList;
extern PICubeSDK_SetBinSkip				ICubeSDK_SetBinSkip;
extern PICubeSDK_GetBinSkip				ICubeSDK_GetBinSkip;
extern PICubeSDK_GetBinSkipList			ICubeSDK_GetBinSkipList;
extern PICubeSDK_SaveToFile				ICubeSDK_SaveToFile;
extern PICubeSDK_SaveAvi   				ICubeSDK_SaveAvi;
extern PICubeSDK_SetTrigger				ICubeSDK_SetTrigger;
extern PICubeSDK_GetTrigger				ICubeSDK_GetTrigger;
extern PICubeSDK_SetCamParameter		ICubeSDK_SetCamParameter;
extern PICubeSDK_GetCamParameter		ICubeSDK_GetCamParameter;
extern PICubeSDK_GetCamParameterRange	ICubeSDK_GetCamParameterRange;
extern PICubeSDK_GetParamAuto			ICubeSDK_GetParamAuto;
extern PICubeSDK_SetParamAuto			ICubeSDK_SetParamAuto;
extern PICubeSDK_SetParamAutoDef		ICubeSDK_SetParamAutoDef;
extern PICubeSDK_SetParamOnePush		ICubeSDK_SetParamOnePush;
extern PICubeSDK_GetName				ICubeSDK_GetName;
extern PICubeSDK_GetBrokenFrames		ICubeSDK_GetBrokenFrames;
extern PICubeSDK_GetGoodFrames			ICubeSDK_GetGoodFrames;
extern PICubeSDK_SetExposure		    ICubeSDK_SetExposure;
extern PICubeSDK_GetExposure		    ICubeSDK_GetExposure;
extern PICubeSDK_GetExposureRange	    ICubeSDK_GetExposureRange;
extern PICubeSDK_SetDisplayMode         ICubeSDK_SetDisplayMode;
extern PICubeSDK_SetResolutionParam		ICubeSDK_SetResolutionParam;

#endif  // LIBFILE

#endif