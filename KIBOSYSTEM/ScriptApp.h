#pragma once

class ScriptApp :public App
{
public:
	ScriptApp(void);
	~ScriptApp(void);

	//クラス実体の作成
	static ScriptApp* CreateInstance();

	//中心点表示
	BOOL CenterPoint(BOOL isVisible);

	//Menu & Button Enable / Disenable
	BOOL EnableWindow(BOOL isEnable);
};
