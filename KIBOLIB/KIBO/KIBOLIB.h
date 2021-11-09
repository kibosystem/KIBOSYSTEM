#pragma once

#include "KIBO.h"
#include "KIBOView.h"
#include "KIBODoc.h"
#include "KIBOFrame.h"
#include "OutputWnd.h"
#include "enum.h"

#include "./Script/ScriptCall.h"
#include "./SCript/App.h"
#include "./SCript/Bitmap.h"
#include "./SCript/Labeling.h"
#include "./SCript/ImageDevice.h"
#include "./SCript/SerialPort.h"
#include "./SCript/USBIO.h"

#include "./LIB/functions.h"
#include "./Lib/AboutDlg.h"
#include "./Lib/typedef.h"
#include "./LIB/FilePlus.h"
#include "./LIB/macro.h"
#include "./LIB/Tiff16.h"

#include "./etc/InputDlg.h"
#include "./Dialog/DialogBarPlus.h"
//#include "./Dialog/ListCtrlPlus.h"

#include "OpenCVInc.h"
#include "LabelingCV.h"

#ifdef DEBUG
#pragma comment(lib,"../x64/Debug/KIBOLIB.lib")
#else
#pragma comment(lib,"../x64/Release/KIBOLIB.lib")
#endif
