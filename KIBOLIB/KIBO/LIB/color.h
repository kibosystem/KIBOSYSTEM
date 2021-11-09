#pragma once

//色設定
#define COLOR_BLACK				RGB(0,0,0)			//黒
#define COLOR_WHITE				RGB(255,255,255)	//白
#define COLOR_DKGRAY			RGB(70,70,70)		//濃い灰色
#define COLOR_GRAY				RGB(132,132,132)	//灰色
#define COLOR_LTGRAY			RGB(198,195,198)	//明るい灰色
#define COLOR_SLTGRAY			RGB(228,228,228)	//さらに明るい灰色
#define COLOR_RED				RGB(255,0,0)		//赤
#define COLOR_BLUE				RGB(0,0,255)		//青
#define COLOR_GREEN				RGB(0,255,0)		//緑
#define COLOR_YELLOW			RGB(255,255,0)		//黄色
#define COLOR_MAZENTA			RGB(255,0,255)		//マゼンタ
#define COLOR_CYAN				RGB(0,255,255)		//シアン
#define COLOR_LIGHTBLUE			RGB(128,128,255)	//ライトブルー
#define COLOR_LIGHTGREEN		RGB( 0x90,0xEE,0x90)//ライトグリーン
#define COLOR_DEEPBLUE			RGB(0,0,128)		//濃い青
#define COLOR_ORANGE			RGB(255,128,0)		//オレンジ
#define COLOR_SEPARATOR			RGB(197,194,184)	//メニューセパレーター
#define COLOR_MEDIUMSLATEBLUE	RGB(123,104,238)	//青灰色
#define COLOR_STEELBLUE			RGB(70,130,180)	
#define COLOR_YELLOWGREEN		RGB(0x9A,0xCD,0x32) //イエローグリーン
#define COLOR_INDIGO			RGB(0x4B,0x00,0x82)	//インディゴ


#define PAINT_RED			RGB(255,  0,  0)	//赤
#define PAINT_ORANGE		RGB(255,106,  0)	//オレンジ
#define PAINT_YELLOW		RGB(255,216,  0)	//黄色
#define PAINT_YELLOWGREEN	RGB(182,255,  0)	//黄緑
#define PAINT_LIGHTGREEN	RGB( 76,255,  0)	//薄緑
#define PAINT_GREEN			RGB(  0,255, 33)	//緑
#define PAINT_GREENBLUE		RGB(  0,255,144)	//青緑
#define PAINT_CYAN			RGB(  0,255,255)	//シアン
#define PAINT_LIGHTBLUE		RGB(  0,148,255)	//水色
#define PAINT_BLUE			RGB(  0, 38,255)	//青
#define PAINT_DEEPPURPLE	RGB( 72,  0,255)	//濃い紫
#define PAINT_PURPLE		RGB(178,  0,255)	//紫
#define PAINT_MAGENTA		RGB(255,  0,220)	//赤紫
#define PAINT_WINERED		RGB(255,  0,110)	//ワインレッド

//キャリブレーション用色設定
enum{
 	 ENUM_RED = 0			//赤
	,ENUM_ORANGE			//オレンジ
	,ENUM_YELLOW			//黄色
	,ENUM_YELLOWGREEN		//黄緑
	,ENUM_LIGHTGREEN		//薄緑
	,ENUM_GREEN				//緑
	,ENUM_GREENBLUE			//青緑
	,ENUM_CYAN				//シアン
	,ENUM_LIGHTBLUE			//水色
	,ENUM_BLUE				//青
	,ENUM_DEEPPURPLE		//濃い紫
	,ENUM_PURPLE			//紫
	,ENUM_MAGENTA			//赤紫
	,ENUM_WINERED			//ワインレッド
	,ENUM_PAINT				//ペイント選択色
};
static LPCTSTR s_lpszPaintColor[] = {
 	 _T("赤")
	,_T("オレンジ")
	,_T("黄色")
	,_T("黄緑")
	,_T("薄緑")
	,_T("緑")
	,_T("青緑")
	,_T("シアン")
	,_T("水色")
	,_T("青")
	,_T("濃い紫")
	,_T("紫")
	,_T("赤紫")
	,_T("ワインレッド")
};
static COLORREF s_crPaintColor[] = {
 	 PAINT_RED			//赤
	,PAINT_ORANGE		//オレンジ
	,PAINT_YELLOW		//黄色
	,PAINT_YELLOWGREEN	//黄緑
	,PAINT_LIGHTGREEN	//薄緑
	,PAINT_GREEN		//緑	
	,PAINT_GREENBLUE	//青緑	
	,PAINT_CYAN			//思案
	,PAINT_LIGHTBLUE	//水色	
	,PAINT_BLUE			//青
	,PAINT_DEEPPURPLE	//濃い紫
	,PAINT_PURPLE		//紫
	,PAINT_MAGENTA		//赤紫
	,PAINT_WINERED		//ワインレッド
};
