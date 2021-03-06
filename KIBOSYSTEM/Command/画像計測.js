//画像計測.js
START:

//ImageDevice.FromBitmap("sample1.jpg");
ImageDevice.KillTimer();


//撮影サイズ変更
g_nW = ImageDevice.Width;  //画像サイズ幅取得
g_nH = ImageDevice.Height; //画像サイズ高さ取得
App.SetViewRate(g_nViewCx / g_nW, g_nViewCy / g_nH); //表示座標の倍率設定

ColorBitmap.FromImageDevice();//カメラ画像をビットマップクラスへ転送
ColorBitmap.SetLineColor(0, 255, 0); //ライン色指定

var nTh = App.GetNum("しきい値");
Labeling.SetBitmap(nTh); //Bitmap Set
var nMinPixels = 1000; //ラベリング最小画素数
var nMaxPixels = 10000000; //ラベリング最大画素数
var imax = Labeling.Exec(nMinPixels, nMaxPixels);
var dbLength = 0;
for (var i = 0; i < imax; i++) {
    var dbWidth = ColorBitmap.CalcWidth(Labeling.Gx(i), Labeling.Gy(i), 10, "max");//幅計算　重心位置を中心に幅10pixel間
    dbLength = dbWidth * g_dbPer;
    App.TextOutput("幅" + (i + 1) + ":" + dbWidth.toFixed(2) + "pixels, " + dbLength.toFixed(2) + "mm");
    ColorBitmap.DrawBitmapLine();//取得幅をビットマップに書き込み
}

for (var i = 0; i < imax; i++) {
    var dbHeight = ColorBitmap.CalcHeight(Labeling.Gx(i), Labeling.Gy(i), 10, "max");//高さ計算　重心位置を中心に幅10pixel間
    dbLength = dbHeight * g_dbPer;
    App.TextOutput("高さ" + (i + 1) + ":" + dbHeight.toFixed(2) + "pixels, " + dbLength.toFixed(2) + "mm");
    ColorBitmap.DrawBitmapLine();//取得幅をビットマップに書き込み
}
ColorBitmap.ToImageDevice();
App.Render(); //画像表示書き換え

END:
