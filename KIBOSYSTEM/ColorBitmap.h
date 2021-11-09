#pragma once

class ColorBitmap : public Bitmap24
{
public:
	ColorBitmap(void);
	~ColorBitmap(void);

	static ColorBitmap* CreateInstance(); //実体の作成

protected:	
//	BOOL ShowBitmap();//ビットマップ表示

};
