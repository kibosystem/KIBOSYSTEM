#pragma once
#include "dib.h"

extern void DrawRectangle(CDIB &dib,CRect &rect,COLORREF color);//指定された矩形をビットマップに描画
extern void DrawRectangles(CDIB &dib,CArray<CRect> &aryRect,COLORREF color);//指定された矩形配列をビットマップに描画

