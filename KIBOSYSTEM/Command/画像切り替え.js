画像切り替え

START:
    App.RemoveAll(); //描画消去
ImageDevice.KillTimer();
var strFile = App.SelectBitmap;     //ビットマップファイル選択
if( strFile != ""){    
    ImageDevice.FromFile(strFile); //カメラ撮影画像の代わりにビットマップを使用
}
App.Render();
END:
