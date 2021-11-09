#pragma once

//�F�ݒ�
#define COLOR_BLACK				RGB(0,0,0)			//��
#define COLOR_WHITE				RGB(255,255,255)	//��
#define COLOR_DKGRAY			RGB(70,70,70)		//�Z���D�F
#define COLOR_GRAY				RGB(132,132,132)	//�D�F
#define COLOR_LTGRAY			RGB(198,195,198)	//���邢�D�F
#define COLOR_SLTGRAY			RGB(228,228,228)	//����ɖ��邢�D�F
#define COLOR_RED				RGB(255,0,0)		//��
#define COLOR_BLUE				RGB(0,0,255)		//��
#define COLOR_GREEN				RGB(0,255,0)		//��
#define COLOR_YELLOW			RGB(255,255,0)		//���F
#define COLOR_MAZENTA			RGB(255,0,255)		//�}�[���^
#define COLOR_CYAN				RGB(0,255,255)		//�V�A��
#define COLOR_LIGHTBLUE			RGB(128,128,255)	//���C�g�u���[
#define COLOR_LIGHTGREEN		RGB( 0x90,0xEE,0x90)//���C�g�O���[��
#define COLOR_DEEPBLUE			RGB(0,0,128)		//�Z����
#define COLOR_ORANGE			RGB(255,128,0)		//�I�����W
#define COLOR_SEPARATOR			RGB(197,194,184)	//���j���[�Z�p���[�^�[
#define COLOR_MEDIUMSLATEBLUE	RGB(123,104,238)	//�D�F
#define COLOR_STEELBLUE			RGB(70,130,180)	
#define COLOR_YELLOWGREEN		RGB(0x9A,0xCD,0x32) //�C�G���[�O���[��
#define COLOR_INDIGO			RGB(0x4B,0x00,0x82)	//�C���f�B�S


#define PAINT_RED			RGB(255,  0,  0)	//��
#define PAINT_ORANGE		RGB(255,106,  0)	//�I�����W
#define PAINT_YELLOW		RGB(255,216,  0)	//���F
#define PAINT_YELLOWGREEN	RGB(182,255,  0)	//����
#define PAINT_LIGHTGREEN	RGB( 76,255,  0)	//����
#define PAINT_GREEN			RGB(  0,255, 33)	//��
#define PAINT_GREENBLUE		RGB(  0,255,144)	//��
#define PAINT_CYAN			RGB(  0,255,255)	//�V�A��
#define PAINT_LIGHTBLUE		RGB(  0,148,255)	//���F
#define PAINT_BLUE			RGB(  0, 38,255)	//��
#define PAINT_DEEPPURPLE	RGB( 72,  0,255)	//�Z����
#define PAINT_PURPLE		RGB(178,  0,255)	//��
#define PAINT_MAGENTA		RGB(255,  0,220)	//�Ԏ�
#define PAINT_WINERED		RGB(255,  0,110)	//���C�����b�h

//�L�����u���[�V�����p�F�ݒ�
enum{
 	 ENUM_RED = 0			//��
	,ENUM_ORANGE			//�I�����W
	,ENUM_YELLOW			//���F
	,ENUM_YELLOWGREEN		//����
	,ENUM_LIGHTGREEN		//����
	,ENUM_GREEN				//��
	,ENUM_GREENBLUE			//��
	,ENUM_CYAN				//�V�A��
	,ENUM_LIGHTBLUE			//���F
	,ENUM_BLUE				//��
	,ENUM_DEEPPURPLE		//�Z����
	,ENUM_PURPLE			//��
	,ENUM_MAGENTA			//�Ԏ�
	,ENUM_WINERED			//���C�����b�h
	,ENUM_PAINT				//�y�C���g�I��F
};
static LPCTSTR s_lpszPaintColor[] = {
 	 _T("��")
	,_T("�I�����W")
	,_T("���F")
	,_T("����")
	,_T("����")
	,_T("��")
	,_T("��")
	,_T("�V�A��")
	,_T("���F")
	,_T("��")
	,_T("�Z����")
	,_T("��")
	,_T("�Ԏ�")
	,_T("���C�����b�h")
};
static COLORREF s_crPaintColor[] = {
 	 PAINT_RED			//��
	,PAINT_ORANGE		//�I�����W
	,PAINT_YELLOW		//���F
	,PAINT_YELLOWGREEN	//����
	,PAINT_LIGHTGREEN	//����
	,PAINT_GREEN		//��	
	,PAINT_GREENBLUE	//��	
	,PAINT_CYAN			//�v��
	,PAINT_LIGHTBLUE	//���F	
	,PAINT_BLUE			//��
	,PAINT_DEEPPURPLE	//�Z����
	,PAINT_PURPLE		//��
	,PAINT_MAGENTA		//�Ԏ�
	,PAINT_WINERED		//���C�����b�h
};
