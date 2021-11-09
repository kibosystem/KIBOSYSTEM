

// INCLUDES ////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "NET_ICube_API.h"

// API /////////////////////////////////////////////////////////////////////////

#ifndef LIBFILE

PICubeSDK_Init						ICubeSDK_Init;
PICubeSDK_Open						ICubeSDK_Open;
PICubeSDK_Close						ICubeSDK_Close;
PICubeSDK_IsOpen					ICubeSDK_IsOpen;
PICubeSDK_GetVersion				ICubeSDK_GetVersion;
PICubeSDK_GetFWVersion				ICubeSDK_GetFWVersion;
PICubeSDK_GetFPGAVersion			ICubeSDK_GetFPGAVersion;
PICubeSDK_GetSerialNum              ICubeSDK_GetSerialNum;
PICubeSDK_SetCallback				ICubeSDK_SetCallback;
PICubeSDK_Start						ICubeSDK_Start;
PICubeSDK_IsStarted					ICubeSDK_IsStarted;
PICubeSDK_Stop						ICubeSDK_Stop;
PICubeSDK_SetResolution				ICubeSDK_SetResolution;
PICubeSDK_GetResolution				ICubeSDK_GetResolution;
PICubeSDK_GetResolutionRange		ICubeSDK_GetResolutionRange;
PICubeSDK_GetSize					ICubeSDK_GetSize;
PICubeSDK_SetMode					ICubeSDK_SetMode;
PICubeSDK_GetModeList				ICubeSDK_GetModeList;
PICubeSDK_SetBinSkip				ICubeSDK_SetBinSkip;
PICubeSDK_GetBinSkip				ICubeSDK_GetBinSkip;
PICubeSDK_GetBinSkipList			ICubeSDK_GetBinSkipList;
PICubeSDK_SaveToFile				ICubeSDK_SaveToFile;
PICubeSDK_SaveAvi   				ICubeSDK_SaveAvi;
PICubeSDK_SetTrigger				ICubeSDK_SetTrigger;
PICubeSDK_GetTrigger				ICubeSDK_GetTrigger;
PICubeSDK_SetCamParameter			ICubeSDK_SetCamParameter;
PICubeSDK_GetCamParameter			ICubeSDK_GetCamParameter;
PICubeSDK_GetCamParameterRange		ICubeSDK_GetCamParameterRange;
PICubeSDK_GetParamAuto				ICubeSDK_GetParamAuto;
PICubeSDK_SetParamAuto				ICubeSDK_SetParamAuto;
PICubeSDK_SetParamAutoDef			ICubeSDK_SetParamAutoDef;
PICubeSDK_SetParamOnePush			ICubeSDK_SetParamOnePush;
PICubeSDK_GetName					ICubeSDK_GetName;
PICubeSDK_GetBrokenFrames			ICubeSDK_GetBrokenFrames;
PICubeSDK_GetGoodFrames				ICubeSDK_GetGoodFrames;
PICubeSDK_GetMode					ICubeSDK_GetMode;
PICubeSDK_SetExposure	    	    ICubeSDK_SetExposure;
PICubeSDK_GetExposure	    	    ICubeSDK_GetExposure;
PICubeSDK_GetExposureRange  	    ICubeSDK_GetExposureRange;
PICubeSDK_SetDisplayMode            ICubeSDK_SetDisplayMode;
PICubeSDK_SetResolutionParam		ICubeSDK_SetResolutionParam;


HMODULE g_hDll = NULL;


BOOL LoadICubeApi(const char* szDllPathname)
{
    g_hDll = LoadLibraryA(szDllPathname);
    if(g_hDll == NULL)
        return IC_ERROR;

	// initialize functions
	#define ICUBE_INITIALIZE_FUNCTION( A) A = (P##A)GetProcAddress( g_hDll, #A)

	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_Init                 );

	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_GetVersion           );
	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_GetFWVersion         );
	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_GetSerialNum         );
	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_GetFPGAVersion       );

	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_Open                 );
	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_Close                );
	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_IsOpen               );

	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_SetCallback          );
	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_Start                );
	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_IsStarted            );
	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_Stop                 );

	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_GetResolution        );
	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_SetResolution        );
	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_GetResolutionRange   );

	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_GetSize              );

	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_SetMode              );
	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_GetMode              );
	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_GetModeList          );

	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_SetBinSkip           );
	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_GetBinSkip           );
	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_GetBinSkipList       );

	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_SaveToFile           );
	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_SaveAvi              );

	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_SetTrigger           );
	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_GetTrigger           );

	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_SetCamParameter      );
	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_GetCamParameter      );
	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_GetCamParameterRange );

	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_GetParamAuto         );
	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_SetParamAuto         );
	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_SetParamAutoDef      );

	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_SetParamOnePush      );

	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_GetName              );

	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_GetBrokenFrames      );
	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_GetGoodFrames        );

	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_SetExposure          );
	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_GetExposure          );
	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_GetExposureRange     );

	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_SetDisplayMode       );

	ICUBE_INITIALIZE_FUNCTION( ICubeSDK_SetResolutionParam   );
   
    return IC_SUCCESS;
}


VOID UnloadICubeApi()
{
    if(g_hDll == NULL) return;

    FreeLibrary(g_hDll);
    g_hDll = NULL;
}

#endif
