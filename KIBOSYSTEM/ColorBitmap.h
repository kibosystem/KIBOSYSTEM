#pragma once

class ColorBitmap : public Bitmap24
{
public:
	ColorBitmap(void);
	~ColorBitmap(void);

	static ColorBitmap* CreateInstance(); //���̂̍쐬

protected:	
//	BOOL ShowBitmap();//�r�b�g�}�b�v�\��

};
