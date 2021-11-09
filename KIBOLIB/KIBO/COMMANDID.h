#pragma once

#define	WM_GRAPH_NOTIFY		(WM_APP + 1)

//WM_USER(0x400) through 0x7FFF
#define ID_USER1	 0x401	//ユーザーコマンド1
#define ID_USER2	 0x402	//ユーザーコマンド2
#define ID_USER3	 0x403	//ユーザーコマンド3
#define ID_USER4	 0x404	//ユーザーコマンド4
#define ID_USER5	 0x405	//ユーザーコマンド5
#define ID_USER6	 0x406	//ユーザーコマンド6
#define ID_USER7	 0x407	//ユーザーコマンド7
#define ID_USER8	 0x408	//ユーザーコマンド8
#define ID_USER9	 0x409	//ユーザーコマンド9
#define ID_USERA	 0x40A	//ユーザーコマンドA
#define ID_USERB	 0x40B	//ユーザーコマンドB
#define ID_USERC	 0x40C	//ユーザーコマンドC
#define ID_USERD	 0x40D	//ユーザーコマンドD

#define WM_RENDER    0x450 //レンダリング要求

#define ID_CALIBRATION  0x500	//キャリブレーション
#define ID_INIT			0x600 //初期化処理
#define ID_BINARY_DLG   0x700	//2値化ダイアログ


//使用するスクリプトクラス
//#define USE_USBIO  //USBIOを使用します//
