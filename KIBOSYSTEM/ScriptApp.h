#pragma once

class ScriptApp :public App
{
public:
	ScriptApp(void);
	~ScriptApp(void);

	//�N���X���̂̍쐬
	static ScriptApp* CreateInstance();

	//���S�_�\��
	BOOL CenterPoint(BOOL isVisible);

	//Menu & Button Enable / Disenable
	BOOL EnableWindow(BOOL isEnable);
};
